//
//  telegram.hpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#ifndef Header_hpp
#define Header_hpp

namespace Telegram {
    struct TelegramConfiguration {
        std::string botToken;
        uint64_t chatID;
    };

    void sendAdvert(const TelegramConfiguration &, const Kufar::Ad &, int64_t chatId);
};
#endif /* Header_hpp */
