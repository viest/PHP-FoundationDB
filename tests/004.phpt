--TEST--
Check for foundationdb presence
--SKIPIF--
<?php if (!extension_loaded("foundationdb")) print "skip"; ?>
--FILE--
<?php
$foundationClient = new \Foundation\Client();

$database = $foundationClient
    ->connection('/etc/foundationdb/fdb.cluster')
    ->database('DB');

$database->set('viest', json_encode([
    'name' => 'JiexinWang',
    'age'  => 22
]));

$result = $database->get('viest');

var_dump($result);
?>
--EXPECT--
string(30) "{"name":"JiexinWang","age":22}"
