$source = $(SolutionDir)\bin\$(Platform)\$(Configuration)\FilesControl\$(TargetName).dll

$destinations = @($(SolutionDir)\bin\$(Platform)\$(Configuration)\Logger, $(SolutionDir)\bin\$(Platform)\$(Configuration)\ServerClientCore, $(SolutionDir)\bin\$(Platform)\$(Configuration)\Tests, $(SolutionDir)\bin\$(Platform)\$(Configuration)\Server, $(SolutionDir)\bin\$(Platform)\$(Configuration)\Client)
foreach($dir in $destinations)
{
    Copy-Item -Path $source -Destination $dir
}