
# include "Move.hh"
# include "Board.hh"

/// TODO: Prevent pieces to move in a way where it would
/// put the kind in check.
namespace {

  void
  differentials(const chess::Coordinates& start,
                const chess::Coordinates& end,
                int& dx,
                int& dy) noexcept
  {
    dx = end.x() - start.x();
    dy = end.y() - start.y();
  }

  bool
  obstructed(const chess::Board& b,
             const chess::Coordinates& start,
             int dx,
             int dy,
             unsigned length) noexcept
  {
    unsigned d = 1u;
    int x = start.x() + dx;
    int y = start.y() + dy;

    while (d < length) {
      chess::pieces::Cell c = b.at(x, y);
      if (c.type != chess::pieces::None) {
        return false;
      }

      x += dx;
      y += dy;
      ++d;
    }

    return true;
  }



}

namespace chess {

  Move::Move(unsigned id):
    m_id(id),

    m_valid(false),
    m_white({pieces::None, Coordinates(0, 0), Coordinates(0, 0), pieces::None, false, false}),
    m_black({pieces::None, Coordinates(0, 0), Coordinates(0, 0), pieces::None, false, false}),

    m_name()
  {
    generateName();
  }

  void
  Move::registerMove(const pieces::Color& c,
                     const pieces::Type& t,
                     const Coordinates& start,
                     const Coordinates& end,
                     const pieces::Type& captured,
                     bool check,
                     bool checkmate) noexcept
  {
    Part* out = &m_white;
    if (c == pieces::Black) {
      out = &m_black;
    }

    out->piece = t;

    out->start = start;
    out->end = end;

    out->captured = captured;

    out->check = check;
    out->checkmate = checkmate;

    if (valid()) {
      generateName();
    }
  }

  bool
  Move::valid() const noexcept {
    return m_white.piece != pieces::None && m_black.piece != pieces::None;
  }

  std::string
  Move::toString() const noexcept {
    return m_name;
  }

  void
  Move::generateName() noexcept {
    // In case of an invalid move, generate an invalid
    // string.
    if (!valid()) {
      // Case of checkmate.
      if (!m_white.checkmate) {
        m_name = std::to_string(m_id) + ". N/A";
        return;
      }
    }

    auto nameMove = [](const Part& p) {
      std::string out = pieces::algebraic(p.piece);

      if (p.captured != pieces::None) {
        if (p.piece == pieces::Pawn) {
          out += cells::file(p.start.asValue());
        }
        out += "x";
      }
      out += cells::toString(p.end.asValue());

      if (p.checkmate) {
        out += "#";
      }
      else if (p.check) {
        out += "+";
      }

      return out;
    };

    // Generate the name from white's move.
    m_name = std::to_string(m_id);
    m_name += ".";

    m_name += nameMove(m_white);

    if (m_white.checkmate) {
      m_name += " 1-0";
    }

    // Generate the name from black's move if needed.
    if (m_black.piece == pieces::None) {
      return;
    }

    m_name += " ";
    m_name += nameMove(m_black);

    if (m_black.checkmate) {
      m_name += " 0-1";
    }
  }

  namespace pieces {

    /// @brief - Pawn related functions.
    namespace pawn {

      inline
      bool
      valid(const Color& c,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept
      {
        // Pawn can move only vertically and diagonally
        // when capturing or doing _en passant_.
        int dx, dy;
        differentials(start, end, dx, dy);

        // Backward moves are prevented.
        if (c == White && dy <= 0) {
          return false;
        }
        if (c == Black && dy >= 0) {
          return false;
        }

        if (dx == 0) {
          // Depending on the color we want to allow the
          // move to the top or to the bottom of the board.
          if (c == White && dy < 0) {
            return false;
          }

          if (c == Black && dy > 0) {
            return false;
          }

          // We only allow two cells move in case the pawn
          // is on its initial line.
          if (c == White && start.y() != 1 && dy > 1) {
            return false;
          }
          if (c == White && dy > 2) {
            return false;
          }

          if (c == Black && start.y() != 6 && dy < -1) {
            return false;
          }
          if (c == Black && dy < -2) {
            return false;
          }

          // Prevent move to an obstructed cell.
          Cell ce = b.at(end.x(), end.y());
          if (ce.type != None) {
            return false;
          }

          return true;
        }

        if (std::abs(dy) > 1) {
          return false;
        }

        Cell ce = b.at(end.x(), end.y());
        if (ce.type != None) {
          // In case there's something, we have to make
          // sure that the piece is of opposite color.
          return ce.color != c;
        }

        // Otherwise, handle the en passant case. According
        // to this page: https://en.wikipedia.org/wiki/En_passant
        // The pawns should be in their fifth row.
        if (c == White && start.y() != 4) {
          return false;
        }
        if (c == Black && start.y() != 3) {
          return false;
        }

        if (dx > 0 && start.x() >= b.w() - 2) {
          return false;
        }
        if (dx < 0 && start.x() < 1) {
          return false;
        }

        // There must be an adjacent pawn in the direction
        // of the move.
        Cell toCapture = b.at(end.x(), start.y());
        if (toCapture.color == c || toCapture.type != Pawn) {
          return false;
        }

        // The last move must have been the double-step move
        // from the opposite pawn.
        /// TODO: Handle last move.
        return false;
      }
    }

    /// @brief - Knight related functions.
    namespace knight {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& /*b*/) noexcept
      {
        // Pawn can move in a weird way and also jump over
        // pieces of the same color (so ignoring any form
        // of obstruction).
        int dx, dy;
        differentials(start, end, dx, dy);

        // Combination of `(dx, dy)` can be:
        // (1, 2) (1, -2)
        // (-1, 2) (-1, -2)
        // (2, 1) (-2, 1)
        // (2, -1), (-2, -1)
        if (std::abs(dx) == 1 && std::abs(dy) == 2) {
          return true;
        }
        if (std::abs(dx) == 2 && std::abs(dy) == 1) {
          return true;
        }

        return false;
      }
    }

    /// @brief - Bishop related functions.
    namespace bishop {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept
      {
        // Rooks can only move diagonally.
        int dx, dy;
        differentials(start, end, dx, dy);

        if (dx == 0 || dy == 0) {
          return false;
        }

        if (std::abs(dx) != std::abs(dy)) {
          return false;
        }

        return obstructed(b, start, (dx > 0 ? 1 : -1), (dy > 0 ? 1 : -1), std::abs(dx));
      }
    }

    /// @brief - Rook related functions.
    namespace rook {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept
      {
        // Rooks can only move horizontally or vertically.
        int dx, dy;
        differentials(start, end, dx, dy);

        if (dx != 0 && dy != 0) {
          return false;
        }

        if (dx != 0) {
          return obstructed(b, start, (dx > 0 ? 1 : -1), 0, std::abs(dx));
        }

        // Assume vertical move.
        return obstructed(b, start, 0, (dy > 0 ? 1 : -1), std::abs(dy));
      }
    }

    /// @brief - Queen related functions.
    namespace queen {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& b) noexcept
      {
        // Queens can move any direction as long as it
        // is a horizontal, vertical or diagonal move.
        int dx, dy;
        differentials(start, end, dx, dy);

        // Vertical move.
        if (dx == 0) {
          return obstructed(b, start, 0, (dy > 0 ? 1 : -1), std::abs(dy));
        }

        if (dy == 0) {
          return obstructed(b, start, (dx > 0 ? 1 : -1), 0, std::abs(dx));
        }

        // Control that the motion is a diagonal move and
        // not some strange combination of horizontal and
        // vertical move.
        if (std::abs(dx) != std::abs(dy)) {
          return false;
        }

        return obstructed(b, start, (dx > 0 ? 1 : -1), (dy > 0 ? 1 : -1), std::abs(dx));
      }
    }

    /// @brief - King related functions.
    namespace king {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& start,
            const Coordinates& end,
            const Board& /*b*/) noexcept
      {
        // Kings can only move one step at a time.
        int dx, dy;
        differentials(start, end, dx, dy);

        if (dx > 1 || dy > 1) {
          return false;
        }

        /// TODO: Prevent king from moving in chess.
        /// TODO: Allow king to castle.
        return true;
      }
    }

    /// @brief - None related functions.
    namespace none {

      inline
      bool
      valid(const Color& /*c*/,
            const Coordinates& /*start*/,
            const Coordinates& /*end*/,
            const Board& /*b*/) noexcept
      {
        return false;
      }
    }

    bool
    valid(const Type& t,
          const Color& c,
          const Coordinates& start,
          const Coordinates& end,
          const Board& b) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::valid(c, start, end, b);
        case Knight:
          return knight::valid(c, start, end, b);
        case Bishop:
          return bishop::valid(c, start, end, b);
        case Rook:
          return rook::valid(c, start, end, b);
        case Queen:
          return queen::valid(c, start, end, b);
        case King:
          return king::valid(c, start, end, b);
        case None:
        default:
          return none::valid(c, start, end, b);
      }
    }

  }
}
