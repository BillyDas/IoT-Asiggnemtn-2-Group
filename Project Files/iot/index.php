<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="utf-8" />
	<title> IOT Fan Temp Set </title>
</head>
 <h1>Smart Fan Assignment Part 1</h1>

	<form action="fanProcess.php" method="POST">
		<fieldset>
          <legend>Fan Temperature Limit</legend>
	<label>Fan Temp Limit:</label> <br />
	<input type="text" id="fanTemp" required="required" maxlength="2" name="fanTemp">
	<br />
	<br />
	<button type="submit">Submit Temp Limit</button>
		</fieldset>
	</form>
</html>