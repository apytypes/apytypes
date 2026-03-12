APyTypes Playground
===================

Welcomde to APyTypes Playground! This is an interactive console with Python and APyTypes
in the web browser based on the `Pyodide <https://pyodide.org/>`_ v0.28.2 terminal
emulator 🐍.

.. raw:: html
    :file: interactive-console.html

.. raw:: html

    <div style="padding: 12px; box-sizing: border-box; background:#222">
        <div align="center">
            <img src="../../_static/matplotlib-logo2.svg" width="200" alt="matplotlib">
            <div align="center" id="MPLcanvas"></div>
        </div>
    </div>
    <script>document.pyodideMplTarget = document.getElementById('MPLcanvas');</script>

.. note::

    The interactive Python console is non-persistent and runs locally in your browser.
    Everything you do here will disappear on exit and no information is sent anywhere.
