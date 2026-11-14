@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"
chcp 65001 > nul
if not exist results mkdir results

echo ===== input\example.txt =====
build\lab6.exe input\example.txt > results\example.txt 2>&1
type results\example.txt

echo ===== input\take_all.txt =====
build\lab6.exe input\take_all.txt > results\take_all.txt 2>&1
type results\take_all.txt

echo ===== input errors =====
if exist results\errors.txt del results\errors.txt
for %%F in (input\errors\*.txt) do (
    echo --- %%F --- >> results\errors.txt
    build\lab6.exe %%F >> results\errors.txt 2>&1
    echo exit code: !errorlevel! >> results\errors.txt
)
type results\errors.txt
echo RUN FINISHED: output saved to results\
