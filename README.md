# PHP-FoundationDB

Refactoring......

------

## PHP code structure

### ApiVersion

```php
namespace Foundation;

/**
 * Class ApiVersion
 *
 * @author  viest
 *
 * @package Foundation
 */
class ApiVersion
{
    /**
     * Select api version
     *
     * @param int $version
     *
     * @return void
     *
     * @author viest
     */
    public function selectApiVersion(int $version)
    {
        //
    }

    /**
     * Max api version
     *
     * @return int
     *
     * @author viest
     */
    public function maxApiVersion()
    {
        //
    }
}
```

### Database

```php
namespace Foundation;

/**
 * Class Database
 *
 * @author  viest
 *
 * @package Foundation
 */
class Database
{
    /**
     * Database constructor.
     *
     * @param string $clusterFilePath
     */
    public function __construct(string $clusterFilePath)
    {
        //
    }

    /**
     * Database destruct
     */
    public function __destruct()
    {
        //
    }

    /**
     * Set database option
     *
     * @param int   $option
     * @param mixed $value
     *
     * @return self
     *
     * @author viest
     */
    public function setOption($option, $value)
    {
        //
    }

    /**
     * Create transaction
     *
     * @param Transaction $transaction
     *
     * @return Transaction
     *
     * @author viest
     */
    public function createTransaction($transaction)
    {
        //
    }
}
```

### Future

```php
namespace Foundation;

/**
 * Class Future
 *
 * @author  viest
 *
 * @package Foundation
 */
class Future
{
    /**
     * Future constructor.
     */
    public function __construct()
    {
        //
    }

    /**
     * Cancel
     *
     * @return self
     *
     * @author viest
     */
    public function cancel()
    {
        //
    }

    /**
     * Destroy
     *
     * @return self
     *
     * @author viest
     */
    public function destroy()
    {
        //
    }

    /**
     * Block until ready
     *
     * @return $this
     *
     * @author viest
     */
    public function blockUntilReady()
    {
        //
    }

    /**
     * Is ready
     *
     * @return bool
     *
     * @author viest
     */
    public function isReady()
    {
        //
    }

    /**
     * Get value
     *
     * @return string|null
     *
     * @author viest
     */
    public function getValue()
    {
        //
    }

    /**
     * Get error
     *
     * @return bool
     *
     * @author viest
     */
    public function getError()
    {
        //
    }
}
```

### Network

```php
namespace Foundation;

/**
 * Class Network
 *
 * @author  viest
 *
 * @package Foundation
 */
class Network
{
    // Deprecated
    // Parameter: (String) IP:PORT
    const OPTION_LOCAL_ADDRESS = 10;

    // Deprecated
    // Parameter: (String) path to cluster file
    const OPTION_CLUSTER_FILE = 20;

    // Enables trace output to a file in a directory of the clients choosing
    // Parameter: (String) path to output directory (or NULL for current working directory)
    const OPTION_TRACE_ENABLE = 30;

    // Sets the maximum size in bytes of a single trace output file. This value should be in the range ``[0, INT64_MAX]``. If the value is set to 0, there is no limit on individual file size. The default is a maximum size of 10,485,760 bytes.
    // Parameter: (Int) max size of a single trace output file
    const OPTION_TRACE_ROLL_SIZE = 31;

    // Sets the maximum size of all the trace output files put together. This value should be in the range ``[0, INT64_MAX]``. If the value is set to 0, there is no limit on the total size of the files. The default is a maximum size of 104,857,600 bytes. If the default roll size is used, this means that a maximum of 10 trace files will be written at a time.
    // Parameter: (Int) max total size of trace files
    const OPTION_TRACE_MAX_LOGS_SIZE = 32;

    // Sets the 'LogGroup' attribute with the specified value for all events in the trace output files. The default log group is 'default'.
    // Parameter: (String) value of the LogGroup attribute
    const OPTION_TRACE_LOG_GROUP = 33;

    // Select the format of the log files. xml (the default) and json are supported.
    // Parameter: (String) Format of trace files
    const OPTION_TRACE_FORMAT = 34;

    // Set internal tuning or debugging knobs
    // Parameter: (String) knob_name=knob_value
    const OPTION_KNOB = 40;

    // Deprecated
    // Parameter: (String) file path or linker-resolved name
    const OPTION_TLS_PLUGIN = 41;

    // Set the certificate chain
    // Parameter: (Bytes) certificates
    const OPTION_TLS_CERT_BYTES = 42;

    // Set the file from which to load the certificate chain
    // Parameter: (String) file path
    const OPTION_TLS_CERT_PATH = 43;

    // Set the private key corresponding to your own certificate
    // Parameter: (Bytes) key
    const OPTION_TLS_KEY_BYTES = 45;

    // Set the file from which to load the private key corresponding to your own certificate
    // Parameter: (String) file path
    const OPTION_TLS_KEY_PATH = 46;

    // Set the peer certificate field verification criteria
    // Parameter: (Bytes) verification pattern
    const OPTION_TLS_VERIFY_PEERS = 47;

    // 
    // Parameter: Option takes no parameter
    const OPTION_BUGGIFY_ENABLE = 48;

    // 
    // Parameter: Option takes no parameter
    const OPTION_BUGGIFY_DISABLE = 49;

    // Set the probability of a BUGGIFY section being active for the current execution.  Only applies to code paths first traversed AFTER this option is changed.
    // Parameter: (Int) probability expressed as a percentage between 0 and 100
    const OPTION_BUGGIFY_SECTION_ACTIVATED_PROBABILITY = 50;

    // Set the probability of an active BUGGIFY section being fired
    // Parameter: (Int) probability expressed as a percentage between 0 and 100
    const OPTION_BUGGIFY_SECTION_FIRED_PROBABILITY = 51;

    // Set the ca bundle
    // Parameter: (Bytes) ca bundle
    const OPTION_TLS_CA_BYTES = 52;

    // Set the file from which to load the certificate authority bundle
    // Parameter: (String) file path
    const OPTION_TLS_CA_PATH = 53;

    // Set the passphrase for encrypted private key. Password should be set before setting the key for the password to be used.
    // Parameter: (String) key passphrase
    const OPTION_TLS_PASSWORD = 54;

    // Disables the multi-version client API and instead uses the local client directly. Must be set before setting up the network.
    // Parameter: Option takes no parameter
    const OPTION_DISABLE_MULTI_VERSION_CLIENT_API = 60;

    // If set, callbacks from external client libraries can be called from threads created by the FoundationDB client library. Otherwise, callbacks will be called from either the thread used to add the callback or the network thread. Setting this option can improve performance when connected using an external client, but may not be safe to use in all environments. Must be set before setting up the network. WARNING: This feature is considered experimental at this time.
    // Parameter: Option takes no parameter
    const OPTION_CALLBACKS_ON_EXTERNAL_THREADS = 61;

    // Adds an external client library for use by the multi-version client API. Must be set before setting up the network.
    // Parameter: (String) path to client library
    const OPTION_EXTERNAL_CLIENT_LIBRARY = 62;

    // Searches the specified path for dynamic libraries and adds them to the list of client libraries for use by the multi-version client API. Must be set before setting up the network.
    // Parameter: (String) path to directory containing client libraries
    const OPTION_EXTERNAL_CLIENT_DIRECTORY = 63;

    // Prevents connections through the local client, allowing only connections through externally loaded client libraries. Intended primarily for testing.
    // Parameter: Option takes no parameter
    const OPTION_DISABLE_LOCAL_CLIENT = 64;

    // Disables logging of client statistics, such as sampled transaction activity.
    // Parameter: Option takes no parameter
    const OPTION_DISABLE_CLIENT_STATISTICS_LOGGING = 70;

    // Enables debugging feature to perform slow task profiling. Requires trace logging to be enabled. WARNING: this feature is not recommended for use in production.
    // Parameter: Option takes no parameter
    const OPTION_ENABLE_SLOW_TASK_PROFILING = 71;

    // Enable client buggify - will make requests randomly fail (intended for client testing)
    // Parameter: Option takes no parameter
    const OPTION_CLIENT_BUGGIFY_ENABLE = 80;

    // Disable client buggify
    // Parameter: Option takes no parameter
    const OPTION_CLIENT_BUGGIFY_DISABLE = 81;

    // Set the probability of a CLIENT_BUGGIFY section being active for the current execution.
    // Parameter: (Int) probability expressed as a percentage between 0 and 100
    const OPTION_CLIENT_BUGGIFY_SECTION_ACTIVATED_PROBABILITY = 82;

    // Set the probability of an active CLIENT_BUGGIFY section being fired. A section will only fire if it was activated
    // Parameter: (Int) probability expressed as a percentage between 0 and 100
    const OPTION_CLIENT_BUGGIFY_SECTION_FIRED_PROBABILITY = 83;

    /**
     * Set network option
     *
     * @param int   $key
     * @param mixed $value
     *
     * @return self
     *
     * @author viest
     */
    public function setOption($key, $value)
    {
        //
    }

    /**
     * Setup
     *
     * @return $this
     *
     * @author viest
     */
    public function setup()
    {
        //
    }

    /**
     * Run
     *
     * @return bool
     *
     * @author viest
     */
    public function run()
    {
        //
    }

    /**
     * Stop
     *
     * @return bool
     *
     * @author viest
     */
    public function stop()
    {
        //
    }
}
```

### Transaction

```php
namespace Foundation;

/**
 * Class Transaction
 *
 * @author  viest
 *
 * @package Foundation
 */
class Transaction
{
    /**
     * Destroy
     *
     * @return bool
     *
     * @author viest
     */
    public function destroy()
    {
        //
    }

    /**
     * Set database option
     *
     * @param int   $option
     * @param mixed $value
     *
     * @return self
     *
     * @author viest
     */
    public function setOption($option, $value)
    {
        //
    }

    /**
     * Set read version
     *
     * @param $version
     *
     * @return $this
     *
     * @author viest
     */
    public function setReadVersion($version)
    {
        //
    }

    /**
     * Get read version
     *
     * @return Future
     *
     * @author viest
     */
    public function getReadVersion()
    {
        //
    }

    /**
     * Get
     *
     * @param string $key
     * @param int    $snapshot
     *
     * @return Future
     *
     * @author viest
     */
    public function get($key, $snapshot)
    {
        //
    }

    /**
     * Set
     *
     * @param string $key
     * @param string $value
     *
     * @return $this
     *
     * @author viest
     */
    public function set($key, $value)
    {
        //
    }

    /**
     * Commit
     *
     * @return Future
     *
     * @author viest
     */
    public function commit()
    {
        //
    }

    /**
     * Reset
     *
     * @return $this
     *
     * @author viest
     */
    public function reset()
    {
        //
    }
}
```