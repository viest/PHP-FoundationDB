--TEST--
Check for foundationdb presence
--SKIPIF--
<?php if (!extension_loaded("foundationdb")) print "skip"; ?>
--FILE--
<?php 
echo "foundationdb extension is available";
?>
--EXPECT--
foundationdb extension is available
