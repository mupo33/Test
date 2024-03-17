Write-Output "Move to 4K dir mkv files to be processed by MADVR HDR"
# Select the target disk
$currentPath = @("F:\")

# Check if the '4K' directory exists
if (!(Test-Path -Path "$currentPath\4K")) {
    # If the '4K' directory does not exist, create it
    New-Item -ItemType Directory -Path "$currentPath\4K"
}

# Find directories with 'UHD' in their name
$directories = Get-ChildItem -Path $currentPath -Recurse -Directory | Where-Object { $_.Name -match 'UHD' }

# Loop through each directory
foreach ($dir in $directories) {
    # Check if a file with the extension '.measurements' exists in the directory
    $measurementFileExists = Test-Path -Path "$($dir.FullName)\*.measurements"

    # If a '.measurements' file does not exist, move .mkv files to the '4K' directory
    if (!$measurementFileExists) {
        # Get .mkv files in the directory
        $mkvFiles = Get-ChildItem -Path $dir.FullName -File | Where-Object { $_.Extension -eq '.mkv' }
		# Move each .mkv file to the '4K' directory
		foreach ($file in $mkvFiles) {
			Move-Item -Path $file.FullName -Destination "$currentPath\4K"
			Write-Output "Moved file: $($file.Name)"
		}
    }
}
# Pause the script
Write-Output "Press any key to continue ..."
$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")