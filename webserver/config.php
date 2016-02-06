<?php
$modes = array("default", "gpio", "gpio_pu", "gpio_pd", "pwm", "timer", "uart", "i2c", "spi", "can", "pruin", "pruout", "qep");
$ocp_dir = '/sys/devices/ocp.3';
?>

<?php
error_reporting(E_ERROR | E_WARNING | E_PARSE);
ini_set('display_errors', '1');

if (!empty($_POST))
{
        foreach ($_POST as $pin => $state)
        {
                $file = glob($ocp_dir . '/' . $pin . '*')[0] . '/state';
                if (!file_exists($file)) continue;
                $cur = trim(file_get_contents($file));
                if (strcmp($state, $cur) != 0)
                {
                        exec('sudo chmod 777  ' . $file);
                        file_put_contents($file, $state);
                }
        }
}

?>

<form method="POST" action="">
<?php

$handle = scandir($ocp_dir, SCANDIR_SORT_ASCENDING);
foreach ($handle as $entry)
{
        if (is_dir($ocp_dir . '/' . $entry) && preg_match('(P\d+_\d+_pinmux.\d+)', $entry) == 1)
        {
                $str = "";
                preg_match('(P\d+_\d+)', $entry, $str);
                echo $str[0] . ': <select name="' . $str[0] . '">';
                $state = trim(file_get_contents($ocp_dir . '/' . $entry . '/state'));
                foreach($modes as $mode)
                {
                        echo '<option value="' . $mode . '"' . (strcmp($mode, $state) == 0 ? 'selected' : '') . '>' . $mode . '</option>';
                }
                echo '</select><br />';
        }
}
?>

<input type="submit" value="Go">
</form>
