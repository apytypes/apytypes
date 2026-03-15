//
// APyTypes Node TypeScript/JavaScript harness for running the APyTypes test suite using
// Pyodide.
//
// (C) 2025-2026: the APyTypes authors
//


import fs from 'node:fs';
import process from 'node:process'
import { loadPyodide } from "pyodide";


async function apytypes_test_harness(apytypes_wheel_name: string) {
    const proc = process;
    const node_ver = `Node.js (${proc.versions.node}) on ${proc.platform}-${proc.arch}`;
    console.log("[ HARNESS ]: APyTypes TypeScript/JavaScript test harness launching");
    console.log(`[ HARNESS ]: JavaScript environment: "${node_ver}"`);
    console.log(`[ HARNESS ]: APyTypes wheel: "${apytypes_wheel_name}"`);
    console.log("[ HARNESS ]: loading Pyodide and providing Python wheels...")
    const pyodide = await loadPyodide();
    await pyodide.loadPackage([`${apytypes_wheel_name}`, "pytest", "numpy"]);

    const home_zip_url: string = "http://localhost:8000/pyodide-home.zip";
    console.log(`[ HARNESS ]: fetching home directory from "${home_zip_url}"`);
    try {
        const resp = await fetch(home_zip_url);
        if (!resp.ok) {
            throw new Error(
                `failed to fetch '${home_zip_url}": ${resp.status} ${resp.statusText}`
            );
        }
        const buf = await resp.arrayBuffer();
        pyodide.unpackArchive(buf, "zip");
        console.log(`[ HARNESS ]: successfully retrieved and unpacked "/home/pyodide"`);
    } catch (e) {
        throw new Error(`failed to provide "/home/pyodide": ${String(e)}`);
    }

    console.log("[ HARNESS ]: launching APyTypes test suite\n");
    const PYTHON_SRC: string = ""
        + 'import pytest\n'
        + 'if pytest.main(["/home/pyodide/lib/test"]):\n'
        + '    raise Exception("APyTypes test bench failure")\n';
    return pyodide.runPython(PYTHON_SRC);
}


async function main()
{
    if (process.argv.length < 3) {
        throw new Error(""
            + "'process.argv.length < 3'; you need to pass at least one candidate of "
            + "the APyTypes WASM-wheel filename as a command-line argument to the test "
            + "harness"
        );
    }

    // Generate a PEP-427 compatible APyTypes wheel regex for the particular Pyodide
    // version (parsed from `pyodide-lock.json`). Pyodide requires the filename of
    // wheels to be on PEP 427 format.
    const lockRaw = await fs.promises.readFile("pyodide-lock.json", "utf8");
    const lock = JSON.parse(lockRaw);
    if (!lock?.info?.python || !lock?.info?.abi_version || !lock?.info?.arch) {
        throw new Error("pyodide-lock.json is missing required fields");
    }
    const pyVer = lock.info.python;
    const abiVer = lock.info.abi_version;
    const abiArch = lock.info.arch;
    const dist = "apytypes"
    const ver = String.raw`[\w.+]+`;
    const pyTag = `cp${pyVer.split('.')[0]}${pyVer.split('.')[1]}`;
    const abiTag = pyTag
    const platformTag = `pyodide_${abiVer}_${abiArch}`
    const pep427RegExp = new RegExp(
        `^${dist}-${ver}-${pyTag}-${abiTag}-${platformTag}\\.whl$`
    )

    // Make sure we have exactly one match of a PEP-427 compatible APyTypes wheel, and
    // then start the test suite.
    const candidates = process.argv.slice(2).filter(
        (filename: string): boolean => pep427RegExp.test(filename)
    );
    if (!candidates.length) {
        throw new Error(""
            + "zero matching PEP-427 compatible APyTypes wheel provided; command-line "
            + "arguments are: "
            + process.argv.toString()
        );
    } else if (candidates.length > 1) {
        throw new Error(""
            + "multiple matching PEP-427 compatible APyTypes wheel provided; "
            + "command-line arguments are:"
            + process.argv.toString()
        );
    } else {
        await apytypes_test_harness(candidates[0]);
    }
}


main().catch((err) => {
    console.error("[ HARNESS ]: ", err instanceof Error ? err.message : err);
    process.exit(1);
});
