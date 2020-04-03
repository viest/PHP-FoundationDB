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

$database->set('viest', json_encode([
    'name' => 'JiexinWang',
    'age'  => 22
]));

$result = $database->get('viest');

var_dump($result);
?>
--EXPECT--
string(30) "{"name":"JiexinWang","age":22}"
