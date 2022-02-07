
# include "Board.hh"
# include <unordered_set>
# include <algorithm>
# include "Round.hh"
# include "Move.hh"

namespace chess {

  Board::Board(int  width,
               int  height) noexcept:
    utils::CoreObject("board"),

    m_width(width),
    m_height(height),

    // White by default.
    m_board(width * height, {pieces::White, pieces::None}),

    m_index(0u),
    m_current(pieces::White),
    m_state({false, false, false, false, false}),
    m_round(m_index),
    m_rounds()
  {
    setService("chess");

    initialize();
  }

  int
  Board::w() const noexcept {
    return m_width;
  }

  int
  Board::h() const noexcept {
    return m_height;
  }

  chess::pieces::Color
  Board::getPlayer() const noexcept {
    return m_current;
  }

  unsigned
  Board::getCurrentRound() const noexcept {
    return m_index;
  }

  chess::Round
  Board::getLastRound() const noexcept {
    if (m_rounds.empty()) {
      return m_round;
    }

    return m_rounds.back();
  }

  bool
  Board::isInCheck(const pieces::Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == pieces::White ? m_state.whiteInCheck : m_state.blackInCheck);
    }

    updateState();

    // Call the same routine.
    return isInCheck(color);
  }

  bool
  Board::isInCheckmate(const pieces::Color& color) const noexcept {
    if (!m_state.dirty) {
      return (color == pieces::White ? m_state.whiteInCheckmate : m_state.blackInCheckmate);
    }

    updateState();

    // Call the same routine.
    return isInCheckmate(color);
  }

  std::unordered_set<Coordinates>
  Board::availablePositions(const Coordinates& coords) noexcept {
    pieces::Cell c = at(coords);
    // Case of an empty piece: no available position.
    if (c.type == pieces::None) {
      return std::unordered_set<Coordinates>();
    }

    return pieces::reachable(c.type, c.color, coords, *this);
  }

  pieces::Cell
  Board::at(int x, int y) const {
    if (x >= m_width || y >= m_height) {
      error(
        "Failed to fetch board piece",
        "Invalid coordinate " + std::to_string(x) + "x" + std::to_string(y)
      );
    }

    return m_board[linear(x, y)];
  }

  pieces::Cell
  Board::at(const Coordinates& c) const {
    return at(c.x(), c.y());
  }

  bool
  Board::move(const Coordinates& start, const Coordinates& end) {
    // Check piece at the start and end location.
    pieces::Cell s = at(start);
    pieces::Cell e = at(end);

    if (s.type == pieces::None) {
      warn("Failed to move from " + start.toString(), "Empty location");
      return false;
    }
    if (e.type != pieces::None && s.color == e.color) {
      warn("Move from " + start.toString() + " would conflict with " + end.toString());
      return false;
    }

    // Prevent wrong pieces to move.
    if (s.color != m_current) {
      warn("Trying to move " + pieces::toString(s.color) + " when " + pieces::toString(m_current) + " should play");
      return false;
    }

    // Make sure the move is valid.
    pieces::Cell sp = m_board[linear(start)];
    if (!pieces::valid(sp.type, s.color, start, end, *this)) {
      warn("Move from " + start.toString() + " to " + end.toString() + " for " + pieces::toString(sp.type) + " is invalid");
      return false;
    }


    // Register the move.
    m_round.registerMove(s.color, s.type, start, end, e.type, false, false);

    if (m_round.valid()) {
      m_rounds.push_back(m_round);
      ++m_index;
      m_round = Round(m_index);

      log("Adding round " + m_rounds.back().toString());
    }

    m_current = (m_current == pieces::White ? pieces::Black : pieces::White);

    // Swap the piece at the starting position with the
    // one at the end position. We also need to erase
    // the data at the starting position.
    m_board[linear(end)] = sp;
    m_board[linear(start)].type = pieces::None;

    // Invalidate cached data.
    m_state.dirty = true;

    return true;
  }

  void
  Board::initialize() noexcept {
    // Whites.
    m_board[cells::A1] = { pieces::White, pieces::Rook };
    m_board[cells::B1] = { pieces::White, pieces::Knight };
    m_board[cells::C1] = { pieces::White, pieces::Bishop };
    m_board[cells::D1] = { pieces::White, pieces::Queen };
    m_board[cells::E1] = { pieces::White, pieces::King };
    m_board[cells::F1] = { pieces::White, pieces::Bishop };
    m_board[cells::G1] = { pieces::White, pieces::Knight };
    m_board[cells::H1] = { pieces::White, pieces::Rook };

    m_board[cells::A2] = { pieces::White, pieces::Pawn };
    m_board[cells::B2] = { pieces::White, pieces::Pawn };
    m_board[cells::C2] = { pieces::White, pieces::Pawn };
    m_board[cells::D2] = { pieces::White, pieces::Pawn };
    m_board[cells::E2] = { pieces::White, pieces::Pawn };
    m_board[cells::F2] = { pieces::White, pieces::Pawn };
    m_board[cells::G2] = { pieces::White, pieces::Pawn };
    m_board[cells::H2] = { pieces::White, pieces::Pawn };

    // Blacks.
    m_board[cells::A7] = { pieces::Black, pieces::Pawn };
    m_board[cells::B7] = { pieces::Black, pieces::Pawn };
    m_board[cells::C7] = { pieces::Black, pieces::Pawn };
    m_board[cells::D7] = { pieces::Black, pieces::Pawn };
    m_board[cells::E7] = { pieces::Black, pieces::Pawn };
    m_board[cells::F7] = { pieces::Black, pieces::Pawn };
    m_board[cells::G7] = { pieces::Black, pieces::Pawn };
    m_board[cells::H7] = { pieces::Black, pieces::Pawn };

    m_board[cells::A8] = { pieces::Black, pieces::Rook };
    m_board[cells::B8] = { pieces::Black, pieces::Knight };
    m_board[cells::C8] = { pieces::Black, pieces::Bishop };
    m_board[cells::D8] = { pieces::Black, pieces::Queen };
    m_board[cells::E8] = { pieces::Black, pieces::King };
    m_board[cells::F8] = { pieces::Black, pieces::Bishop };
    m_board[cells::G8] = { pieces::Black, pieces::Knight };
    m_board[cells::H8] = { pieces::Black, pieces::Rook };

    // Handle custom initialization.
    initializeCustom();
  }

  void
  Board::initializeCustom() noexcept {
    // Insert custom logic to initialize the board.
  }

  inline
  unsigned
  Board::linear(int x, int y) const noexcept {
    return y * m_width + x;
  }

  inline
  unsigned
  Board::linear(const Coordinates& c) const noexcept {
    return linear(c.x(), c.y());
  }

  inline
  void
  Board::updateState() const noexcept {
    // Gather the list of pieces remaining for white
    // and black. Also, keep track of the position of
    // the king as it will be useful later.
    std::vector<std::pair<pieces::Type, Coordinates>> wp;
    std::vector<std::pair<pieces::Type, Coordinates>> bp;

    // Note that we don't check whether the kings are
    // found as it is guaranteed by other means.
    Coordinates wKing(0, 0);
    Coordinates bKing(0, 0);

    for (int y = 0 ; y < m_height ; ++y) {
      for (int x = 0 ; x < m_width ; ++x) {
        const pieces::Cell& c = m_board[linear(x, y)];

        if (c.type == pieces::None) {
          continue;
        }

        if (c.color == pieces::White) {
          wp.push_back(std::make_pair(c.type, Coordinates(x, y)));
        }
        else {
          bp.push_back(std::make_pair(c.type, Coordinates(x, y)));
        }

        if (c.type == pieces::King && c.color == pieces::White) {
          wKing = Coordinates(x, y);
        }
        if (c.type == pieces::King && c.color == pieces::Black) {
          bKing = Coordinates(x, y);
        }
      }
    }

    // Compute the list of threats generated by each
    // piece for both colors.
    std::unordered_set<Coordinates> wThreats;
    for (unsigned id = 0u ; id < wp.size() ; ++id) {
      std::unordered_set<Coordinates> t = pieces::reachable(
        wp[id].first,
        pieces::White,
        wp[id].second,
        *this
      );

      wThreats.merge(t);
    }

    std::unordered_set<Coordinates> bThreats;
    for (unsigned id = 0u ; id < bp.size() ; ++id) {
      std::unordered_set<Coordinates> t = pieces::reachable(
        bp[id].first,
        pieces::Black,
        bp[id].second,
        *this
      );

      bThreats.merge(t);
    }

    // Check if the king of each color stands among the
    // list of threatened cells.
    m_state.whiteInCheck = bThreats.count(wKing) > 0u;
    m_state.blackInCheck = wThreats.count(bKing) > 0u;

    if (m_state.whiteInCheck) {
      warn("White is in check");
    }
    if (m_state.blackInCheck) {
      warn("Black is in check");
    }

    /// TODO: Handle checkmate by comparing if any of the
    /// accessible squares for the king is not in threat.
    /// We also need to take into consideration possible
    /// moves of other pieces.

    if (m_state.whiteInCheckmate) {
      warn("White is in checkmate");
    }
    if (m_state.blackInCheckmate) {
      warn("Black is in checkmate");
    }

    // The state has been updated.
    m_state.dirty = false;
  }

}
