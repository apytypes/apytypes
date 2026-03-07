import type { Terminal } from "@xterm/xterm";
import type { PyodideAPI } from "pyodide";


/**
 * Read-Eval-Print Loop state for an interactive APyTypes-Pyodide console
 */
export type REPLConsole = {
    buffer: string,
    prompt: string,
    cursorIdx: number,
    history: Array<string>,
    historyIdx: number | null,
};


/**
 * Compute the longest common prefix among a list of strings.
 *
 * @param strings: `Array<string>`
 *     The strings to find longest common prefix among.
 */
export function longestCommonPrefix(
    strings: Array<string>
) {
    if (!strings.length) return "";
    let prefix = strings[0];
    for (let i = 1; i < strings.length; i++) {
        while (!strings[i].startsWith(prefix)) {
            prefix = prefix.slice(0, -1);
            if (!prefix) return "";
        }
    }
    return prefix;
}


/**
 * Refresh the xterm.js terminal line with the content of the REPLConsole.
 *
 * @param term: `Terminal`
 *     The associated xterm.js terminal.
 *
 * @param repl: `REPLConsole`
 *     The associated Read-Eval-Print Loop console.
 */
export function refreshLine(
    term: Terminal,
    repl: REPLConsole,
) {
    // Write left part, save cursor, write right part, clear, restore cursor.
    const clearCommand = "\x1b[0K";
    const leftPart = prompt + repl.buffer.slice(0, repl.cursorIdx);
    const rightPart = repl.buffer.slice(repl.cursorIdx);
    term.write(`\x1b[0G${leftPart}\x1b[s${rightPart}${clearCommand}\x1b[u`);
}


/**
 * Store a new buffer in the REPL and refresh the associated xterm.js terminal.
 *
 * @param term: `Terminal`
 *     The associated xterm.js terminal.
 *
 * @param repl: `REPLConsole`
 *     The associated Read-Eval-Print Loop console.
 *
 * @param newBuffer: `string`
 *     The new buffer to store into the REPL
 *
 * @param newCursorIndex: `number | null`
 *     Optional new location for the xterm.js cursor. The cursor is placed at the very
 *     end of the buffer if un-used.
 */
export function setBuffer(
    term: Terminal,
    repl: REPLConsole,
    newBuffer: string,
    newCursorIndex: number | null = null
) {
    repl.buffer = newBuffer;
    if (newCursorIndex === null) {
        repl.cursorIdx = repl.buffer.length;
    } else {
        repl.cursorIdx = Math.max(0, Math.min(newCursorIndex, repl.buffer.length));
    }
    refreshLine(term, repl);
}


/**
 * Display auto-completion suggestions in the xterm.js terminal and redraw the REPL
 * console buffer.
 *
 * @param term: `Terminal`
 *     The associated xterm.js terminal.
 *
 * @param repl: `REPLConsole`
 *     The associated Read-Eval-Print Loop console.
 *
 * @param comps: `Array<string>`
 *     The auto-completion suggestions to display.
 */
export function showCompletions(
    term: Terminal,
    repl: REPLConsole,
    comps: Array<string>,
) {
    const sep = "    ";
    const longestCompletion = comps.reduce((max, s) => Math.max(max, s.length), 0);
    const itemColWidth = longestCompletion + sep.length;
    const itemsPerCol = Math.max(1, Math.floor(term.cols / itemColWidth));

    term.write("\r\n");
    for (const [i, str] of comps.entries()) {
        term.write(str.padEnd(itemColWidth));
        if ((i % itemsPerCol) == itemsPerCol - 1) {
            term.write("\r\n");
        }
    }
    term.write("\r\n");
    refreshLine(term, repl);
}

export function handleTabCompletion(
    term: Terminal,
    repl: REPLConsole,
    pyconsole: any
) {
    const before = repl.buffer.slice(0, repl.cursorIdx);
    const after = repl.buffer.slice(repl.cursorIdx);

    // If cursor is in leading whitespace, treat TAB as indentation
    if (before.trim() === "") {
        const insert = "    ";
        setBuffer(term, repl, before + insert + after, repl.cursorIdx + insert.length);
        return;
    }

    // Ask Pyodide for completions for the text before the cursor
    const res = pyconsole.complete(before); // returns (list[str], start:int)
    const js = res.toJs();
    const comps = js[0] || [];
    const start = js[1] ?? before.length;
    res.destroy?.();

    if (!comps.length) {
        term.write("\x07"); // bell
        return;
    }

    const currentToken = before.slice(start);
    if (comps.length === 1) {
        const completed = before.slice(0, start) + comps[0];
        setBuffer(term, repl, completed + after, completed.length);
        return;
    }

    // Multiple completions: extend to common prefix if possible
    const lcp = longestCommonPrefix(comps);
    if (lcp && lcp.length > currentToken.length) {
        const completed = before.slice(0, start) + lcp;
        setBuffer(term, repl, completed + after, completed.length);
        return;
    }

    // Otherwise show options
    showCompletions(term, repl, comps);
}

function addToHistory(repl: REPLConsole, command: string) {
    const trimmed = command.trimEnd();
    if (!trimmed) return;
    const last = repl.history[repl.history.length - 1];
    if (last !== trimmed) {
        repl.history.push(trimmed);
        localStorage.setItem("0_commands", JSON.stringify(history));
    }
}



export async function execLine(
    term: Terminal,
    repl: REPLConsole,
    pyconsole: any,
    await_fut: any,
    repr_shorten: any,
    pyodide: PyodideAPI,
    line: string,
) {
    line = line.replace(/\u00a0/g, " ");
    if (line === "clear") {
        term.clear();
        return;
    }
    const ps1 = ">>> ";
    const ps2 = "... ";

    const fut = pyconsole.push(line);
    switch (fut.syntax_check) {
        case "syntax-error":
            term.write(`\x1b[31m${fut.formatted_error.trimEnd()}\x1b[0m`);
            term.write("\r\n");
            repl.prompt = ps1;
            addToHistory(repl, line);
            repl.historyIdx = null;
            fut.destroy();
            break;
        case "incomplete":
            repl.prompt = ps2;
            addToHistory(repl, line);
            repl.historyIdx = null;
            return;
        case "complete":
            repl.prompt = ps1;
            try {
                const wrapped = await_fut(fut);
                const [value] = await wrapped;
                if (value !== undefined) {
                    const output = repr_shorten.callKwargs(value, {
                        separator: "\r\n<long output truncated>\r\n",
                    });
                    term.write(output);
                    term.write("\r\n");
                }
                if (value instanceof pyodide.ffi.PyProxy) {
                    value.destroy();
                }
                wrapped.destroy();
            } catch (e) {
                const msg = fut.formatted_error || e.message;
                term.write(`\x1b[31m${String(msg).trimEnd()}\x1b[0m`);
                term.write("\r\n");
            } finally {
                fut.destroy();
            }
            addToHistory(repl, line);
            repl.historyIdx = null;
            break;
        default:
            term.write(`\r\nUnexpected syntax_check value: ${fut.syntax_check}`);
    }
}
