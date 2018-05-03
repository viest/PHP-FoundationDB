# PHP-FoundationDB

#### Example

```php
$foundationClient = new \Foundation\Client();

$database = $foundationClient
    ->connection('/usr/local/etc/foundationdb/fdb.cluster')
    ->database('DB');

$database->set('viest', json_encode([
    'name' => 'JiexinWang',
    'age' => 22
]));

$user       = $database->get('viest');
$rangeArray = $database->range(0, 2);
```