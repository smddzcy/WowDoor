<?php

/**
 * Class DB
 */
class DB
{
    /**
     * @var PDO $instance Singleton DB reference
     */
    private static $db = null;

    /**
     * Returns the DB instance
     *
     * @return PDO The DB instance.
     */
    public static function getInstance()
    {
        if (static::$db === null) {
            static::$db = new PDO("sqlsrv:server=wowdoor.database.windows.net,1433;atabase = wowdoor", "wowdoor", "wow!1233211");
            static::$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            static::$db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);
        }
        return static::$db;
    }

    /**
     * DB constructor.
     * Protected to make sure it can't be instantiated with new keyword
     */
    protected function __construct()
    {
    }

    /**
     * Private to make sure it's not clonable
     */
    private function __clone()
    {
    }

    /**
     * Private to make sure it's not unserializable
     */
    private function __wakeup()
    {
    }

}