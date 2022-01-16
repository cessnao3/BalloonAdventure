# Remove old build items
Remove-Item .\build -Recurse -ErrorAction Ignore
Remove-Item .\dist -Recurse -ErrorAction Ignore
Remove-Item .\__pycache__ -Recurse -ErrorAction Ignore
Remove-Item .\*.spec -Recurse -ErrorAction Ignore

# Enter the correct submodule
.\venv\Scripts\activate

# Run pyinstaller
pyinstaller.exe --noconsole .\balloon-adventure.py

# Copy over the resources folder
Copy-Item -Recurse .\resources\ .\dist\balloon-adventure\
