--TEST--
Check for foundationdb presence
--SKIPIF--
<?php if (!extension_loaded("foundationdb")) print "skip"; ?>
--FILE--
<?php 
$database = $foundation
    ->connection('/etc/foundationdb/fdb.cluster')
    ->database('DB');

var_dump($foundationClient);
?>
--EXPECT--
object(Foundation\Client)#1 (0) {
}

