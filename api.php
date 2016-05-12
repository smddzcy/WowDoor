<?php


header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET');
header("Access-Control-Allow-Headers: X-Requested-With");

error_reporting(E_ALL); // Development
//error_reporting(E_ERROR);

require_once 'vendor/luracast/restler/vendor/restler.php';

use Luracast\Restler\Restler;

$r = new Restler();
$r->addAPIClass('IotController', 'wowdoor');
$r->handle();
