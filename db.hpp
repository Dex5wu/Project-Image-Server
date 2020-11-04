#pragma once
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <string>
#include <memory>
#include <mysql/mysql.h>
#include <json/json.h>
namespace image_system {
static MYSQL* MySQLInit() {
    MYSQL* connect_fd = mysql_init(NULL);
    if (mysql_real_connect(connect_fd, "127.0.0.1", "root", "12345678",
                           "image_system", 3306, NULL, 0) == NULL) {
        printf("连接失败! %s\n", mysql_error(connect_fd));
        return NULL;

    }
    mysql_set_character_set(connect_fd, "utf8");
    return connect_fd;

}
static void MySQLRelease(MYSQL* mysql) {
    mysql_close(mysql);

}
class ImageTable {
public:
    ImageTable(MYSQL* mysql) : mysql_(mysql) { }
    bool Insert(const Json::Value& image) {
        char sql[4096] = { 0  };
        sprintf(sql, "insert into image_table values(null, '%s', %d, '%s','%s', '%s', '%s')",
                image["image_name"].asCString(), image["size"].asInt(), image["upload_time"].asCString(),
                image["md5"].asCString(), image["content_type"].asCString(),
                image["path"].asCString());
        int ret = mysql_query(mysql_, sql);
        if (ret != 0) {
            printf("执行 sql 失败! sql=%s, %s\n", sql, mysql_error(mysql_));
            return false;

        }
        return true;

    }
    bool SelectAll(Json::Value* images) {
        char sql[1024 * 4] = { 0  };
        // 可以根据 tag_id 来筛选结果
        sprintf(sql, "select * from image_table");
        int ret = mysql_query(mysql_, sql);
        if (ret != 0) {
            printf("执行 sql 失败! %s\n", mysql_error(mysql_));
            return false;
        }
        MYSQL_RES* result = mysql_store_result(mysql_);
        if (result == NULL) {
            printf("获取结果失败! %s\n", mysql_error(mysql_));
            return false;
        }
        int rows = mysql_num_rows(result);
        for (int i = 0; i < rows; ++i) {
            MYSQL_ROW row = mysql_fetch_row(result);
            Json::Value image;
            image["image_id"] = atoi(row[0]);
            image["image_name"] = row[1];
            image["size"] = atoi(row[2]);
            image["upload_time"] = row[3];
            image["md5"] = row[4];
            image["content_type"] = row[5];
            image["path"] = row[6];
            images->append(image);
        }
        return true;
    }
    bool SelectOne(int32_t image_id, Json::Value* image) {
        char sql[1024 * 4] = { 0  };
        sprintf(sql, "select * from image_table where image_id = %d", image_id);
        int ret = mysql_query(mysql_, sql);
        if (ret != 0) {
            printf("执行 sql 失败! %s\n", mysql_error(mysql_));
            return false;
        }
        MYSQL_RES* result = mysql_store_result(mysql_);
        if (result == NULL) {
            printf("获取结果失败! %s\n", mysql_error(mysql_));
            return false;
        }
        int rows = mysql_num_rows(result);
        if (rows != 1) {
            printf("查找结果不为 1 条. rows = %d!\n", rows);
            return false;
        }
        MYSQL_ROW row = mysql_fetch_row(result);
        (*image)["image_id"] = atoi(row[0]);
        (*image)["image_name"] = row[1];
        (*image)["size"] = atoi(row[2]);
        (*image)["upload_time"] = row[3];
        (*image)["md5"] = row[4];
        (*image)["content_type"] = row[5];
        (*image)["path"] = row[6];
        return true;
    }
    bool Delete(int image_id) {
        char sql[1024 * 4] = { 0  };
        sprintf(sql, "delete from image_table where image_id=%d", image_id);
        int ret = mysql_query(mysql_, sql);
        if (ret != 0) {
            printf("执行 sql 失败! sql=%s, %s\n", sql, mysql_error(mysql_));
            return false;
        }
        return true;
    }
private:
    MYSQL* mysql_;
};
} // end blog_system
