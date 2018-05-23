# PHP-FoundationDB

FoundationDB is a distributed database designed to handle large volumes of structured data across clusters of commodity servers. It organizes data as an ordered key-value store and employs ACID transactions for all operations. It is especially well-suited for read/write workloads but also has excellent performance for write-intensive workloads.

### Open Database

```php
$foundationClient = new \Foundation\Client();

$database = $foundationClient
    ->connection('/usr/local/etc/foundationdb/fdb.cluster')
    ->database('DB');
```

### Insert

#### Grammar

```php
bool set(string $key, string $value);
```

#### Example

```php
$database->set('viest', json_encode([
    'name' => 'JiexinWang',
    'age' => 22
]));
```

### Find

#### Grammar

```php
string get(string $key);
```

#### Example

```php
$user = $database->get('viest');
```

### Delete

#### Grammar

```php
bool clear(string $key);
```

#### Example

```php
$database->clear('viest')
```

### Range

#### Grammar

```php
array range(int $startIndex, int $endIndex);
```

#### Example

```php
$rangeArray = $database->range(0, 2);
```