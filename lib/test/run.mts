//
// APyTypes TypeScript/JavaScript harness for running the APyTypes test suite using
// Pyodide.
//
// (C) 2025: the APyTypes authors
//


import { loadPyodide } from "pyodide";

function get_simple_env_str(): string {
  const g = globalThis as any;
  if (g.process?.versions?.node) {
    const proc = g.process
    return `Node.js (${proc.versions.node}) on ${proc.platform}-${proc.arch}`
  } else if (g.Deno) {
    return `Deno (${g.Deno.version})`
  } else if (g.Bun) {
    return `Bun (${g.Bun.version})`
  } else if (typeof g.window !== "undefined") {
    // Browser
    if ((navigator as any).userAgentData) {
      const brands = (navigator as any).userAgentData.brands;
      return `Browser (${navigator.userAgent}) (${brands})`
    } else {
      return `Browser (${navigator.userAgent})`
    }
  } else {
    return "unknown"
  }
}


async function run_apytypes_test_harness(apytypes_wheel_name: string) {
    console.log("[ HARNESS ]: APyTypes TypeScript/JavaScript test harness launching");
    console.log(`[ HARNESS ]: JavaScript environment: "${get_simple_env_str()}"`);
    console.log(`[ HARNESS ]: APyTypes wheel: "${apytypes_wheel_name}"`);
    console.log("[ HARNESS ]: loading Pyodide")
    let pyodide = await loadPyodide();

    console.log("[ HARNESS ]: providing Python wheels");
    await pyodide.loadPackage([
        `${apytypes_wheel_name}`,
        "pytest",
        "numpy"
    ]);


    const home_zip_url: string = "http://localhost:8000/pyodide-home.zip";
    console.log(`[ HARNESS ]: fetching home directory via "${home_zip_url}"`);
    try {
        const resp = await fetch(home_zip_url);
        const buf = await resp.arrayBuffer();
        pyodide.unpackArchive(buf, "zip");
        console.log(`[ HARNESS ]: successfully retrieved and unpacked "/home/pyodide"`);
    } catch (e) {
        console.log(`[ HARNESS ]: failed to provide "/home/pyodide"`);
        console.log(`[ HARNESS ]: FATAL ERROR, exiting ungracefully...`);
        throw e;
    }

    console.log("[ HARNESS ]: launching APyTypes test suite\n");
    const PYTHON_SRC: string = `
        import pytest
        if pytest.main(["/home/pyodide/lib/test"]):
            raise Exception("APyTypes test bench failure")
    `
    return pyodide.runPython(PYTHON_SRC);
}

// Extract the appropriate APyTypes Pyodide wheel from the installed Pyodide
// distribution lockfile and run the test suite harness on that wheel
import fs from 'node:fs'
fs.readFile('pyodide-lock.json', 'utf8', function(err: any, data: string) {
    if (err) throw err;
    const pyodide_info = JSON.parse(data).info;
    const py_ver = pyodide_info.python;
    const abi_ver = pyodide_info.abi_version;
    const abi_arch = pyodide_info.arch;

    // Generate a PEP 427-compatible APyTypes wheel regex
    const dist = "apytypes"
    const ver = String.raw`[\w\d.+]+`;
    const py_tag = `cp${py_ver.split('.')[0]}${py_ver.split('.')[1]}`;
    const abi_tag = py_tag
    const platform_tag = `pyodide_${abi_ver}_${abi_arch}`
    const wheel_regex = new RegExp(
        `${dist}-${ver}-${py_tag}-${abi_tag}-${platform_tag}.whl`
    )

    // Find the appropriate APyTypes Pyodide wheel name(s)
    const candidates = fs.readdirSync('.').filter(
        (filename: string): boolean => wheel_regex.test(filename)
    )

    // Run the APyTypes test harness on the first matching candidate wheel name
    run_apytypes_test_harness(candidates[0])
});
