#include <gtest/gtest.h>
#include <parser/Parser.h>

TEST(ParserTest, PathNavigation)
{
    std::string_view content = R"(
        settings {
            graphics {
                resolution { x: 1920 y: 1080 }
                fullscreen: true
            }
            levels: ["forest", "cave", "boss_room"]
        }
    )";

    auto root = matt::parser::Parser::parseContent(content);
    EXPECT_EQ(root.get("settings.graphics.resolution.x").asInt(), 1920);
    EXPECT_EQ(root.get("settings.graphics.resolution.y").asInt(), 1080);
    EXPECT_EQ(root.get("settings.graphics.fullscreen").asBool(), true);
    EXPECT_EQ(root["settings"]["levels"][0].asString(), "forest");
    EXPECT_EQ(root["settings"]["levels"][1].asString(), "cave");
    EXPECT_EQ(root["settings"]["levels"][2].asString(), "boss_room");
}

TEST(ParserTest, MultipleMaps)
{
    std::string_view content = R"(
        meta { version: 1 }
        video { fps: 60 }
        audio { volume: 10.5 }
    )";

    auto root = matt::parser::Parser::parseContent(content);

    // All three should exist in the root Value
    EXPECT_EQ(root.get("meta.version").asInt(), 1);
    EXPECT_EQ(root.get("video.fps").asInt(), 60);
    EXPECT_EQ(root.get("audio.volume").asDouble(), 10.5);
}