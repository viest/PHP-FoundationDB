--TEST--
Check for foundationdb presence
--SKIPIF--
<?php if (!extension_loaded("foundationdb")) print "skip"; ?>
--FILE--
<?php
$foundationClient = new \Foundation\Client();

$clusterFile = '/etc/foundationdb/fdb.cluster';

if (PHP_OS === 'Darwin') {
    $clusterFile = '/usr/local/etc/foundationdb/fdb.cluster';
}

$database = $foundationClient
    ->connection($clusterFile)
    ->database('DB');

var_dump($foundationClient);
?>
--EXPECT--
object(Foundation\Client)#1 (0) {
}

