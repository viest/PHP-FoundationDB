--TEST--
Check for foundationdb presence
--SKIPIF--
<?php if (!extension_loaded("foundationdb")) print "skip"; ?>
--FILE--
<?php
for ($index = 0; $index < 2; $index++) {
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

    var_dump($database->get('viest'));
}

$database->clear('viest');

var_dump($database->get('viest'));
?>
--EXPECT--
string(30) "{"name":"JiexinWang","age":22}"
string(30) "{"name":"JiexinWang","age":22}"
NULL