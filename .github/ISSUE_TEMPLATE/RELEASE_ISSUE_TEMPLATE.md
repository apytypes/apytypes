# Release checklist

<!--
Link to open issues and pull requests below.
-->

## Open issues

- [ ]

## Open pull requests

- [ ]

## Before release

- [ ] Close issues and pull requests associated with the milestone,
or push them to a future release.
- [ ] Update `CHANGELOG.md` with the release changes.

## Do the release

- [ ] **On the release commit**, make an annotated tag
(e.g., `git tag -a v0.5.0 -m "v0.5.0"`).
- [ ] Push the new release tag: `git push --tags`.
This step triggers building and uploading of wheels.
- [ ] Wait for CI/CD to build all wheels and publish them to PyPI.
- [ ] Make a new GitHub release from the annotated tag.

## After release: update wheel in interactive console

- [ ] Copy WASM wheels from the build artifacts to [github.com/apytypes/wasm32-wheels](https://github.com/apytypes/wasm32-wheels).
- [ ] Update the interactive-console wheel-link:

### apytypes/docs/interactive-console.html:L72

```javascript
await micropip.install(
    ""
    + "https://raw.githubusercontent.com/apytypes/wasm32-wheels/main/"
    + "apytypes-<version>-cp313-cp313-pyodide_2025_0_wasm32.whl"
)
