// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include "game.h"
#include <cassert>

Game::Game() : m_currentPlayer(PLAYER_UNKNOWN), m_player1(0), m_player2(0) {
}

void Game::setPlayer1(Player* player) {
  m_player1 = player;
}

void Game::setPlayer2(Player* player) {
  m_player2 = player;
}

void Game::play(bool printSteps, std::ostream& os) {
  m_currentPlayer = PLAYER_1;

  // Cannot start game without players.
  assert(m_player1);
  assert(m_player2);

  PlayerType winner = PLAYER_UNKNOWN;

  for (;;) {
    if (printSteps) {
      m_board.print(os, true);
    }

    size_t move = 9999;
    switch (m_currentPlayer) {
      case PLAYER_1: {
        if (printSteps) {
          os << "Player 1 (X)" << std::endl;
        }

        size_t move = 9;
        size_t invalidMoveCount = 0;
        do {
          move = m_player1->getMove(m_board, 'X');
          ++invalidMoveCount;
          //if (invalidMoveCount > 9) {
          //  assert(false);
          //  return;
          //}
        } while (!m_board.setMove(move, 'X'));

        m_currentPlayer = PLAYER_2;
      } break;

      case PLAYER_2: {
        if (printSteps) {
          os << "Player 2 (O)" << std::endl;
        }

        size_t move = 9;
        size_t invalidMoveCount = 0;
        do {
          move = m_player2->getMove(m_board, 'O');
          ++invalidMoveCount;
          //if (invalidMoveCount > 9) {
          //  assert(false);
          //  return;
          //}
        } while (!m_board.setMove(move, 'O'));

        m_currentPlayer = PLAYER_1;
      } break;

      default:
        break;
    }

    char winnerCell = m_board.getWinner();
    if (winnerCell == 'X') {
      winner = PLAYER_1;
      break;
    } else if (winnerCell == 'O') {
      winner = PLAYER_2;
      break;
    } else if (m_board.isFull())
      break;

    if (printSteps) {
      os << std::endl;
    }
  }

  if (printSteps) {
    os << std::endl;
    os << "Winning board:" << std::endl;
  }

  // Print the final board without instructions.
  if (printSteps) {
    m_board.print(os, false);
  }

  if (winner == PLAYER_1) {
    m_player1->reportWinner(m_board, Player::Win);
    m_player2->reportWinner(m_board, Player::Lose);
    if (printSteps) {
      os << "Player 1 wins!" << std::endl;
    }
  } else if (winner == PLAYER_2) {
    m_player1->reportWinner(m_board, Player::Lose);
    m_player2->reportWinner(m_board, Player::Win);
    if (printSteps) {
      os << "Player2 wins!" << std::endl;
    }
  } else {
    m_player1->reportWinner(m_board, Player::Draw);
    m_player2->reportWinner(m_board, Player::Draw);
    if (printSteps) {
      os << "Game is a draw!" << std::endl;
    }
  }

  // Reset the game.
  m_board = Board();
}
