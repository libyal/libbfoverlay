# Tests tools functions and types.

$ToolsTests = "output signal"
$OptionSets = "" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${ToolsTests} -split " ")
{
	# Split will return an array of a single empty string when ToolsTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTestBinary ${TestExecutablesDirectory} "bfoverlay_test_tools_${TestName}"

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}
