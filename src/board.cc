#include "board.hpp"

const bool Y0_DEBUG = true;

Board::Board ()
{
  cursor_x_   = 0;
  cursor_y_   = 0;
  open_count_ = 0;
  lose_flag_  = false;

  int mode_index = select_mode ();
  std::tie (board_height_, board_width_) = BOARD_HEIGHT_WIDTH_[mode_index];
  number_of_bomb_ = NUMBER_OF_BOMB_FOR_LEVEL_[mode_index];

  cells_array_.resize (board_height_);
  for (int i = 0; i < board_height_; ++i) {
    cells_array_.at (i).resize (board_width_);
  }

  for (int i = 0; i < board_height_; ++i) {
    for (int j = 0; j < board_width_; ++j) {
      cells_array_[i][j] = {true, false, false};
    }
  }

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  int bomb_counter = 0;
  while (bomb_counter < number_of_bomb_) {
    int x = engine () % board_width_;
    int y = engine () % board_height_;
    bomb_counter += static_cast<int> (!cells_array_[y][x].bomb_);
    cells_array_[y][x].bomb_ = true;
  }
}

int Board::select_mode ()
{
  std::system ("clear");
  auto is_out_menue_range = [] (int select_mode_number) {
    return 1 > select_mode_number || 3 < select_mode_number;
  };

  int select_mode_number = -1;
  bool end_select = false;
  std::cout << MENUE_MESSAGE_ << std::flush;
  while (!end_select) {
    std::string s;
    std::getline (std::cin, s);
    if (s.size () != 0)
      select_mode_number = static_cast<int> (s.at (0)) - static_cast<int> ('0');

    if (Y0_DEBUG) {
      std::cout << "Board::select_mode select_mode_number: " <<
      select_mode_number << std::endl;
      std::cout << "end_select: " << end_select << std::endl;
    }

    if (!(end_select = !is_out_menue_range (select_mode_number))) {
      std::cout << RETRY_SELECT_MESSAGE_ << std::endl;; 
      std::cout << MENUE_MESSAGE_ << std::flush;
    }
  }
  return select_mode_number - 1;
}

void Board::game ()
{
  while (!is_end ()) {
    print_board ();
    print_command ();
    operate_board ();
  }
}

void Board::print_board ()
{
  std::system ("clear");
  for (int i = 0; i < board_height_; ++i) {
    for (int j = 0; j < board_width_; ++j) {
      if (i == cursor_y_ && j == cursor_x_) {
        std::cout << CURSOR_MARKER_ << std::flush;
      }
      else {
        std::cout << (
            cells_array_[i][j].flag_ ?
            FLAG_MARKER_ : cells_array_[i][j].mine_ ?
            MINE_MARKER_ : cells_array_[i][j].bomb_ ?
            BOMB_MARKER_ : number_of_arround_bomb (j, i)) << std::flush;
      }
    }
    puts ("");
  }
  if (Y0_DEBUG) {
    std::cout << "open_count_: " << open_count_ << std::endl;
    std::cout << "cursor: " << cursor_y_ << " " << cursor_x_ << std::endl;
  }
}

void Board::print_command ()
{
  std::cout << COMMAND_LIST_ << std::endl;
}

bool Board::is_out_of_board (int y, int x)
{
  return 0 > y || board_height_ <= y ||
    0 > x || board_width_ <= x;
}

int Board::get_number_of_arround_bomb (int y, int x) 
{
  int number_of_arround_bomb_count = 0;
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      if (0 == i && 0 == j)
        continue;
      if (is_out_of_board (y + i, x + j))
        continue;

      number_of_arround_bomb_count += static_cast<int> (cells_array_[y + i][x + j].bomb_);
    }
  }
  return number_of_arround_bomb_count;
}

std::string Board::number_of_arround_bomb (int x, int y)
{
  int number_of_arround_bomb_count = get_number_of_arround_bomb (y, x);

  if (number_of_arround_bomb_count == 0)
    return {BLANK_MARKER_.begin (), BLANK_MARKER_.end ()};

  std::string return_string = "０";
  return_string[2] += number_of_arround_bomb_count;

  return return_string;
}

void Board::operate_board ()
{
  switch (getch ()) {
    case 'w':
      cursor_y_ = std::max (0, cursor_y_ - 1);
      break;
    case 's':
      cursor_y_ = std::min (board_height_ - 1, cursor_y_ + 1);
      break;
    case 'a':
      cursor_x_ = std::max (0, cursor_x_ - 1);
      break;
    case 'd':
      cursor_x_ = std::min (board_width_ - 1, cursor_x_ + 1);
      break;
    case 'f':
      if (cells_array_[cursor_y_][cursor_x_].mine_)
        cells_array_[cursor_y_][cursor_x_].flag_ ^= true;
      else
        cells_array_[cursor_y_][cursor_x_].flag_  = false;
      break;
    case 'k':
      if (cells_array_[cursor_y_][cursor_x_].flag_)
        break;
      if (cells_array_[cursor_y_][cursor_x_].mine_) {
        open_arround_select_cells (cursor_y_, cursor_x_);
        cells_array_[cursor_y_][cursor_x_].mine_ = false;
      }

      lose_flag_ = cells_array_[cursor_y_][cursor_x_].bomb_;
  }
}

void Board::open_arround_select_cells (int y, int x)
{
  if (is_out_of_board (y, x) || cells_array_[y][x].bomb_ || 
      !cells_array_[y][x].mine_)
    return;

  if (Y0_DEBUG) {
    std::cout << "auto_open: " << y << " " << x << std::endl;
    std::cout << "is_out_of_board: : " << is_out_of_board (y, x) << "points: " << y << " " << x << std::endl;
  }

  cells_array_[y][x].mine_ = false;
  ++open_count_;

  if (0 != get_number_of_arround_bomb (y, x)) 
    return;

  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      open_arround_select_cells (y + i, x + j);
    }
  }
}

void Board::all_open ()
{
  for (int i = 0; i < board_height_; ++i) {
    for (int j = 0; j < board_width_; ++j) {
      cells_array_[i][j].mine_ = false;
      cells_array_[i][j].flag_ = false;
    }
  }
  print_board ();
}

bool Board::is_end ()
{
  if (is_clear ()) {
    end_processing (CLEAR_MESSAGE_);
    return true;
  }
  if (is_lose ()) {
    end_processing (LOSE_MESSAGE_);
    return true;
  }
  return false;
}

void Board::end_processing (std::string_view message)
{
  cursor_y_ = cursor_x_ = -1;
  all_open ();
  std::cout << message << std::endl;
  getch ();
}

bool Board::is_clear ()
{
  return board_height_ * board_width_ - number_of_bomb_ == open_count_;
}

bool Board::is_lose ()
{
  return lose_flag_;
}
