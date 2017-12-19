#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <list>
#include <iostream>
#include "../Game.h"

using namespace ::testing;
using namespace std;

class CoutRedirect
{
public:
    CoutRedirect(streambuf *new_buf) :
        _old_buf(cout.rdbuf(new_buf))
        {}
        ~CoutRedirect() { cout.rdbuf(_old_buf); }
    private:
        streambuf *_old_buf;
    };

    namespace {
        void add_players(Game &game, const list<string> &players)
        {
            for (const auto &player : players) {
                game.add(player);
            }
        }
    }

    TEST(MyModifiedTriviaGame, CallingRollGeneratesOutputIdenticalToOriginal)
    {
        // Given a list of players
        list<string> players {"Bjarne", "Herb", "Andrei"};
        // And an original game with these players
        Game gm_game;
        add_players(gm_game, players);
        // And a modified game with these players
        Game modified_game;
        add_players(modified_game, players);
        // And a dice roll
        const int DICE_ROLL {4};

        // When a game round is played in the original game
        cout << "## Start capturing GM roll()" << endl;
        string gm_capture;
        {
            stringstream capture_buf;
            CoutRedirect rd(capture_buf.rdbuf());
            gm_game.roll(DICE_ROLL);
            gm_capture = capture_buf.str();
        }
        // And a game round is played in the modified game
        cout << "## Start capturing modified roll()" << endl;
        string modified_capture;
        {
            stringstream capture_buf;
            CoutRedirect rd(capture_buf.rdbuf());
            modified_game.roll_modified(DICE_ROLL);
            modified_capture = capture_buf.str();
        }

        // Then the output of the GM is not empty
        EXPECT_TRUE(gm_capture.size() > 0);
        // And both must be identical
        EXPECT_THAT(modified_capture, StrEq(gm_capture));
    }
