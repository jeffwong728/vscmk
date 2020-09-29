%~d0
cd %~dp0
set ISODATE=%date:~6,4%-%date:~0,2%-%date:~3,2%
"C:\Program Files\7-Zip\7z.exe" a -tzip vscmk-%ISODATE%.zip .\*.* -xr!build -xr!.git