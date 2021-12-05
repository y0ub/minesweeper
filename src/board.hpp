#ifndef MINESWEEPER_SRC_BOARD_HPP
#define MINESWEEPER_SRC_BOARD_HPP

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include "../../../../c++/util/y0_conio.h"

class Board
{
  private:
    static const int BOARD_HEIGHT_LEVEL_1_ = 9;
    static const int BOARD_WIDTH_LEVEL_1_  = 9;
    static const int BOARD_HEIGHT_LEVEL_2_ = 9;
    static const int BOARD_WIDTH_LEVEL_2_  = 18;
    static const int BOARD_HEIGHT_LEVEL_3_ = 18;
    static const int BOARD_WIDTH_LEVEL_3_  = 18;

    static constexpr std::pair<int, int> BOARD_HEIGHT_WIDTH_[] = {
      std::make_pair (BOARD_HEIGHT_LEVEL_1_, BOARD_WIDTH_LEVEL_1_),
      std::make_pair (BOARD_HEIGHT_LEVEL_2_, BOARD_WIDTH_LEVEL_2_),
      std::make_pair (BOARD_HEIGHT_LEVEL_3_, BOARD_WIDTH_LEVEL_3_)};

    static constexpr int NUMBER_OF_BOMB_FOR_LEVEL_[] = {10, 20, 40};
    int number_of_bomb_ = 10;

    static constexpr std::string_view MINE_MARKER_   = "◻️ ";
    static constexpr std::string_view BOMB_MARKER_   = "＊";
    static constexpr std::string_view FLAG_MARKER_   = "＊";
    static constexpr std::string_view CURSOR_MARKER_ = "　";
    static constexpr std::string_view BLANK_MARKER_  = "・";

    static constexpr std::string_view MENUE_MESSAGE_ =
      "please select mode\n"
      "1:9x9\n"
      "2:9x18\n"
      "3:18x18:";

    static constexpr std::string_view RETRY_SELECT_MESSAGE_  = "please input much number";

    static constexpr std::string_view COMMAND_LIST_ =
      "w:up\n"
      "s:down\n"
      "a:left\n"
      "d:right\n"
      "f:flag\n"
      "k:open\n";

    static constexpr std::string_view LOSE_MESSAGE_  = "you lose";
    static constexpr std::string_view CLEAR_MESSAGE_ = "you win!!!!11!";

    int board_height_;
    int board_width_;

    int cursor_x_;
    int cursor_y_;
    int open_count_;
    bool lose_flag_;

    struct CellStatus_
    {
      bool mine_;
      bool bomb_;
      bool flag_;
    };

    std::vector <std::vector <CellStatus_> > cells_array_;

    int select_mode ();
    void print_board ();
    void print_command ();
    void operate_board ();
    void open_arround_select_cells (int y, int x);
    int get_number_of_arround_bomb (int y, int x);
    std::string number_of_arround_bomb (int x, int y);
    bool is_out_of_board (int y, int x);
    void all_open ();
    bool is_end ();
    void end_processing (std::string_view message);
    bool is_clear ();
    bool is_lose ();

  public:
    Board ();
    void game ();
};

#endif
