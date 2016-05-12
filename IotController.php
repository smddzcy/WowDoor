<?php

use Luracast\Restler\RestException;

/**
 * Class IotController
 */
class IotController
{

    /**
     * Authorize with unique device ID, also checks if the device is paired
     *
     * @param string $id ID of the device
     * @return boolean True or false
     * @throws RestException DB couldn't be reached
     * @url GET is-device-registered/{id}
     */
    public static function authorizeWithUniqueID($id)
    {
        $db = DB::getInstance();
        $query = $db->prepare("SELECT * FROM devices WHERE device_id = :id AND paired = 1");
        if ($query->execute([
                ":id" => $id
            ]) === false
        ) {
            throw new RestException(412, "DB connection error.");
        }
        $query->fetch();
        return $query->rowCount() > 0;
    }

    /**
     * Pairs the device
     *
     * @param string $id ID of the device
     * @return boolean True or false
     * @throws RestException DB couldn't be reached
     * @url GET pair-device/{id}
     */
    public static function pairDevice($id)
    {
        $db = DB::getInstance();
        $query = $db->prepare("SELECT * FROM devices WHERE device_id = :id");
        if ($query->execute([
                ":id" => $id
            ]) === false
        ) {
            throw new RestException(412, "DB connection error.");
        }
        $query->fetch();
        if ($query->rowCount() > 0) {
            $query = $db->prepare("UPDATE devices SET paired = 1 WHERE device_id = :id");
            if ($query->execute([
                    ":id" => $id
                ]) === false
            ) {
                throw new RestException(412, "DB connection error.");
            }
            return true;
        }
        return false;
    }

    /**
     * Unpairs the device
     *
     * @param string $id ID of the device
     * @return boolean True or false
     * @throws RestException DB couldn't be reached
     * @url GET unpair-device/{id}
     */
    public static function unpairDevice($id)
    {
        $db = DB::getInstance();
        $query = $db->prepare("SELECT * FROM devices WHERE device_id = :id");
        if ($query->execute([
                ":id" => $id
            ]) === false
        ) {
            throw new RestException(412, "DB connection error.");
        }
        $query->fetch();
        if ($query->rowCount() > 0) {
            $query = $db->prepare("UPDATE devices SET paired = 0 WHERE device_id = :id");
            if ($query->execute([
                    ":id" => $id
                ]) === false
            ) {
                throw new RestException(412, "DB connection error.");
            }
            return true;
        }
        return false;
    }

    /**
     * Sends a door open request
     *
     * @param string $id ID of the device
     * @throws RestException DB couldn't be reached
     * @url GET request-door-open/{id}
     */
    public static function doorOpenRequest($id)
    {

        $db = DB::getInstance();
        $query = $db->prepare("SELECT * FROM requests WHERE device_id = :id");
        if ($query->execute([
                ":id" => $id
            ]) === false
        ) {
            throw new RestException(412, "DB connection error.");
        }
        $query->fetch();
        if ($query->rowCount() == 0) {
            $query = $db->prepare("INSERT INTO requests (device_id) VALUES (:id)");
            if ($query->execute([
                    ":id" => $id
                ]) === false
            ) {
                throw new RestException(412, "DB connection error.");
            }
        }
    }

    /**
     * Checks if door open request is sent
     *
     * @param string $id ID of the device
     * @return boolean True or false
     * @throws RestException DB couldn't be reached
     * @url GET is-door-request-sent/{id}
     */
    public static function isDoorOpenRequestSent($id)
    {
        $db = DB::getInstance();
        $query = $db->prepare("SELECT * FROM requests WHERE device_id = :id");
        if ($query->execute([
                ":id" => $id
            ]) === false
        ) {
            throw new RestException(412, "DB connection error.");
        }
        $query->fetch();
        if ($query->rowCount() == 0) return false;
        $query = $db->prepare("DELETE FROM requests WHERE device_id = :id");
        if ($query->execute([
                ":id" => $id
            ]) === false
        ) {
            throw new RestException(412, "DB connection error.");
        }
        return true;
    }

}