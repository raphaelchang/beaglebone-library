<?php

error_reporting(E_ERROR | E_WARNING | E_PARSE);
ini_set('display_errors', '1');

$gpio_dir = "/sys/class/gpio/gpio[0-9]*";
$pwm_dir = "/sys/class/pwm/pwm[0-9]*";
$adc_dir = "/sys/bus/iio/devices/iio:device0/in_voltage[0-9]*_raw";
$uart_dir = "/dev/ttyO[0-9]*";
$i2c_dir = "/dev/i2c-[0-9]*";
$spi_dir = "/dev/spidev[0-9]*.[0-9]*";
?>
<a href="">Refresh</a>
<h2>GPIO</h2>
<?php
if (isset($_POST['export-gpio']) && is_numeric($_POST['export-gpio']))
{
        exec('sudo chmod 777  ' . dirname($gpio_dir) . '/export');
        file_put_contents(dirname($gpio_dir) . '/export', $_POST['export-gpio']);
}
?>
<?php
$handle = glob($gpio_dir, GLOB_ONLYDIR);
foreach ($handle as $entry)
{
        $pin = basename($entry);
        $direction = trim(file_get_contents($entry . '/direction'));
        $value = trim(file_get_contents($entry . '/value'));
        if (isset($_POST[$pin . '-value']))
        {
                if (strcmp($value, $_POST[$pin . '-value']) != 0)
                {
                        exec('sudo chmod 777  ' . $entry . '/value');
                        file_put_contents($entry . '/value', $_POST[$pin . '-value']);
                }
        }

        if (isset($_POST[$pin . '-direction']))
        {
                if (strcmp($direction, $_POST[$pin . '-direction']) != 0)
                {
                        exec('sudo chmod 777  ' . $entry . '/direction');
                        file_put_contents($entry . '/direction', $_POST[$pin . '-direction']);
                }
        }
        $value = trim(file_get_contents($entry . '/value'));
        $direction = trim(file_get_contents($entry . '/direction'));

        echo '<form method="POST" action="">';
        echo $pin . ': ';
        echo '<input name="' . $pin . '-direction" value="' . $direction . '">';
        echo '<input name="' . $pin . '-value" value="' . $value . '"' . ($direction == 'in' ? 'disabled' : '') . '>';
        echo '<input type="submit" value="Go"></form>';
}

?>

<form method="POST" action="">
<input name="export-gpio" size="2">
<input type="submit" value="Export Pin">
</form>

<h2>PWM</h2>
<?php
if (isset($_POST['export-pwm']) && is_numeric($_POST['export-pwm']))
{
        exec('sudo chmod 777  ' . dirname($pwm_dir) . '/export');
        file_put_contents(dirname($pwm_dir) . '/export', $_POST['export-pwm']);
        exec('sudo chmod 777  ' . dirname($pwm_dir) . '/pwm' . $_POST['export-pwm'] . '/run');
        file_put_contents(dirname($pwm_dir) . '/pwm' . $_POST['export-pwm'] . '/run', 1);
}
?>
<?php
$handle = glob($pwm_dir, GLOB_ONLYDIR);
foreach ($handle as $entry)
{
        $pin = basename($entry);
        $period = trim(file_get_contents($entry . '/period_ns'));
        $duty = trim(file_get_contents($entry . '/duty_ns'));
        if (isset($_POST[$pin . '-duty']))
        {
                if (strcmp($duty, $_POST[$pin . '-duty']) != 0)
                {
                        exec('sudo chmod 777  ' . $entry . '/duty_ns');
                        file_put_contents($entry . '/duty_ns', $_POST[$pin . '-duty']);
                }
        }

        if (isset($_POST[$pin . '-period']))
        {
                if (strcmp($period, $_POST[$pin . '-period']) != 0)
                {
                        exec('sudo chmod 777  ' . $entry . '/period_ns');
                        file_put_contents($entry . '/period_ns', $_POST[$pin . '-period']);
                }
        }
        $period = trim(file_get_contents($entry . '/period_ns'));
        $duty = trim(file_get_contents($entry . '/duty_ns'));

        echo '<form method="POST" action="">';
        echo $pin . ': ';
        echo '<input name="' . $pin . '-period" value="' . $period . '">';
        echo '<input name="' . $pin . '-duty" value="' . $duty . '">';
        echo '<input type="submit" value="Write"></form>';
}

?>

<form method="POST" action="">
<input name="export-pwm" size="2">
<input type="submit" value="Export Pin">
</form>

<h2>ADC</h2>

<?php
$handle = glob($adc_dir);
foreach ($handle as $entry)
{
        $pin = basename($entry);
        $value = file_get_contents($entry);
        echo $pin . ': ' . $value . '<br />';
}
?>

<h2>I2C</h2>
<?php
$handle = glob($i2c_dir);
foreach ($handle as $entry)
{
	$pin = basename($entry);
	$value = '';
	if (isset($_POST[$pin . '-action']))
	{
		if ($_POST[$pin . '-action'] == 'Write')
		{
			if (isset($_POST[$pin . '-chip-address']) && isset($_POST[$pin . '-data-address']) && isset($_POST[$pin . '-value'])
				&& $_POST[$pin . '-chip-address'] !== '' && $_POST[$pin . '-data-address'] !== '' && $_POST[$pin . '-value'] !== '')
			{
				$numbers = array();
				preg_match_all('/\d+/', $pin, $numbers);
				$bus = end($numbers[0]);
				echo exec('sudo i2cset -y ' . $bus . ' ' . $_POST[$pin . '-chip-address'] . ' ' . $_POST[$pin . '-data-address'] . ' ' . $_POST[$pin . '-value']);
			}
		}
		else if ($_POST[$pin . '-action'] == 'Read')
		{
			if (isset($_POST[$pin . '-chip-address']) && $_POST[$pin . '-chip-address'] !== '')
			{
				$numbers = array();
				preg_match_all('/\d+/', $pin, $numbers);
				$bus = end($numbers[0]);
				if (isset($_POST[$pin . '-data-address']) && $_POST[$pin . '-data-address'] !== '')
					$value = exec('sudo i2cget -y ' . $bus . ' ' . $_POST[$pin . '-chip-address'] . ' ' . $_POST[$pin . '-data-address']);
				else
					$value = exec('sudo i2cget -y ' . $bus . ' ' . $_POST[$pin . '-chip-address']);
			}
		}
	}

	echo '<form method="POST" action="">';
	echo $pin . ': ';
	echo '<input name="' . $pin . '-chip-address">';
	echo '<input name="' . $pin . '-data-address">';
	echo '<input name="' . $pin . '-value" value="' . $value . '">';
	echo '<input type="submit" name="' . $pin . '-action" value="Write">';
	echo '<input type="submit" name="' . $pin . '-action" value="Read"></form>';
}
?>
<a href="config.php">Configure Pins</a>
