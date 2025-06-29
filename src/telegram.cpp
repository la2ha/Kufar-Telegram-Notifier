//
//  telegram.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#include <iostream>
#include "kufar.hpp"
#include "telegram.hpp"
#include "networking.hpp"
#include "json.hpp"

namespace Telegram {
    using namespace std;
    using namespace Networking;
    using nlohmann::json;

    const short int MAX_IMAGES_IN_GROUP = 10;
    string makeImageGroupJSON(const vector<string> &images, const string &caption) {
        json j_array = json::array();
        for (int i = 0; (i < images.size()) && (i < MAX_IMAGES_IN_GROUP); i++){
            json j_list = json::object({
                    {"type", "photo"},
                    {"media", images[i]}
            });
            
            if (i == 0) {
                j_list.push_back({"caption", caption});
            }
            j_array.push_back(j_list);
        }
        
        return j_array.dump();
    }

    void sendAdvert(const TelegramConfiguration &telegramConfiguration, const Kufar::Ad &ad, int64_t chatId) {
        string formattedTime = ctime(&ad.date);
        formattedTime.pop_back();
        
        string text = "";
        
        if (ad.tag.has_value()) {
            text += "#" + ad.tag.value() + "\n";
        }
        
        text += "Название: " + ad.title + "\n"
                "Дата: " + formattedTime + "\n"
                "Цена: " + to_string(ad.price / 100) + " BYN\n\n"
                "Имя продавца: " + ad.sellerName + "\n"
                "Номер телефона не скрыт: " + (ad.phoneNumberIsVisible ? "Да" : "Нет") + "\n"
                "Ссылка: " + ad.link;
        
        string url = "https://api.telegram.org/bot" + telegramConfiguration.botToken;
        if (!ad.images.empty()) {
            url += "/sendMediaGroup?chat_id=" + to_string(chatId) + "&"
                   "media=" + urlEncode(makeImageGroupJSON(ad.images, text));
        } else {
            url += "/sendPhoto?chat_id=" + to_string(chatId) + "&"
                   "caption=" + urlEncode(text) + "&"
                   "photo=https://via.placeholder.com/1080";
            /*url += "/sendMessage?chat_id=" + telegramConfiguration.chatID_or_Username + "&"
                   "text=" + urlEncode("[No Photo]\n\n" + text);*/
        }
        
        getJSONFromURL(url);
    }
};
