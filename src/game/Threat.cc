
# include "Threat.hh"
# include "Board.hh"

namespace chess {
  namespace pieces {

    /// @brief - Pawn related functions.
    namespace pawn {

      inline
      std::vector<Coordinates>
      threaten(const Coordinates& p,
               const Color& c,
               const Board& b) noexcept
      {
        // A pawn threatens the two cells in diagonally in front
        // of it as long as they are part of the board.
        std::vector<Coordinates> out;

        int row = p.y() + 1;
        if (c == Black) {
          row = p.y() - 1;
        }

        if (p.x() > 0 && row >= 0 && row < b.h()) {
          out.push_back(Coordinates(p.x() - 1, row));
        }
        if (p.x() < b.w() - 1 && row >= 0 && row < b.h()) {
          out.push_back(Coordinates(p.x() + 1, row));
        }

        return out;
      }
    }

    /// @brief - Knight related functions.
    namespace knight {

      inline
      std::vector<Coordinates>
      threaten(const Coordinates& p,
               const Color& /*c*/,
               const Board& b) noexcept
      {
        // A knight threatens a certain amount of cells around
        // it with the following deltas compared to the current
        // position:
        // (1, 2) (1, -2)
        // (-1, 2) (-1, -2)
        // (2, 1) (-2, 1)
        // (2, -1), (-2, -1)
        std::vector<Coordinates> out;

        std::vector<Coordinates> coords = {
          Coordinates(p.x() + 1, p.y() + 2),
          Coordinates(p.x() + 2, p.y() + 1),

          Coordinates(p.x() + 2, p.y() - 1),
          Coordinates(p.x() + 1, p.y() - 2),

          Coordinates(p.x() - 1, p.y() - 2),
          Coordinates(p.x() - 2, p.y() - 1),

          Coordinates(p.x() - 2, p.y() + 1),
          Coordinates(p.x() - 1, p.y() + 2),
        };

        for (unsigned id = 0u ; id < coords.size() ; ++id) {
          // Ignore invalid coordinates.
          if (coords[id].x() < 0 || coords[id].x() >= b.w()) {
            continue;
          }
          if (coords[id].y() < 0 || coords[id].y() >= b.h()) {
            continue;
          }

          out.push_back(coords[id]);
        }

        return out;
      }
    }

    /// @brief - Bishop related functions.
    namespace bishop {

      inline
      std::vector<Coordinates>
      threaten(const Coordinates& p,
               const Color& /*c*/,
               const Board& b) noexcept
      {
        // A bishop threatens all the diagonal cells from
        // its position, as long but up to the blocked cell
        // or the edge of the board.
        std::vector<Coordinates> out;

        // Top right diagonal.
        int x = p.x() + 1;
        int y = p.y() + 1;
        bool blocked = false;

        while (x < b.w() && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++x;
          ++y;
        }

        // Top left diagonal.
        x = p.x() - 1;
        y = p.y() + 1;
        blocked = false;

        while (x >= 0 && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --x;
          ++y;
        }

        // Bottom left diagonal.
        x = p.x() - 1;
        y = p.y() - 1;
        blocked = false;

        while (x >= 0 && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --x;
          --y;
        }

        // Bottom right diagonal.
        x = p.x() + 1;
        y = p.y() - 1;
        blocked = false;

        while (x < b.w() && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++x;
          --y;
        }

        return out;
      }
    }

    /// @brief - Rook related functions.
    namespace rook {

      inline
      std::vector<Coordinates>
      threaten(const Coordinates& p,
               const Color& /*c*/,
               const Board& b) noexcept
      {
        // A rook threatens all the horizontal and vertical
        // cells from its position, as long but up to the
        // blocked cell or the edge of the board.
        std::vector<Coordinates> out;

        // Positive horizontal.
        int delta = p.x() + 1;
        bool blocked = false;

        while (delta < b.w() && !blocked) {
          Coordinates co(delta, p.y());
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++delta;
        }

        // Negative horizontal.
        delta = p.x() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(delta, p.y());
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --delta;
        }

        // Positive vertical.
        delta  = p.y() + 1;
        blocked = false;

        while (delta < b.h() && !blocked) {
          Coordinates co(p.x(), delta);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++delta;
        }

        // Negative vertical.
        delta = p.y() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(p.x(), delta);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --delta;
        }

        return out;
      }
    }

    /// @brief - Queen related functions.
    namespace queen {

      inline
      std::vector<Coordinates>
      threaten(const Coordinates& p,
               const Color& /*c*/,
               const Board& b) noexcept
      {
        // A queen threatens all the cells horizontally but
        // also vertically and diagonally from its position,
        // as long but up to the blocked cell or the edge of
        // the board.
        std::vector<Coordinates> out;

        // Positive horizontal.
        int delta = p.x() + 1;
        bool blocked = false;

        while (delta < b.w() && !blocked) {
          Coordinates co(delta, p.y());
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++delta;
        }

        // Negative horizontal.
        delta = p.x() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(delta, p.y());
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --delta;
        }

        // Positive vertical.
        delta  = p.y() + 1;
        blocked = false;

        while (delta < b.h() && !blocked) {
          Coordinates co(p.x(), delta);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++delta;
        }

        // Negative vertical.
        delta = p.y() - 1;
        blocked = false;

        while (delta >= 0 && !blocked) {
          Coordinates co(p.x(), delta);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --delta;
        }

        // Top right diagonal.
        int x = p.x() + 1;
        int y = p.y() + 1;
        blocked = false;

        while (x < b.w() && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++x;
          ++y;
        }

        // Top left diagonal.
        x = p.x() - 1;
        y = p.y() + 1;
        blocked = false;

        while (x >= 0 && y < b.h() && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --x;
          ++y;
        }

        // Bottom left diagonal.
        x = p.x() - 1;
        y = p.y() - 1;
        blocked = false;

        while (x >= 0 && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          --x;
          --y;
        }

        // Bottom right diagonal.
        x = p.x() + 1;
        y = p.y() - 1;
        blocked = false;

        while (x < b.w() && y >= 0 && !blocked) {
          Coordinates co(x, y);
          Cell c = b.at(co);
          out.push_back(co);

          blocked = (c.type != None);
          ++x;
          --y;
        }

        return out;
      }
    }

    /// @brief - King related functions.
    namespace king {

      inline
      std::vector<Coordinates>
      threaten(const Coordinates& p,
               const Color& /*c*/,
               const Board& b) noexcept
      {
        // A king threatens all adjacent cells as long as
        // they are part of the board.
        std::vector<Coordinates> out;

        for (int y = p.y() - 1 ; y <= p.y() + 1 ; ++y) {
          if (y < 0 || y >= b.h()) {
            continue;
          }

          for (int x = p.x() - 1 ; x <= p.x() + 1 ; ++x) {
            if (x < 0 || x >= b.w()) {
              continue;
            }

            // Ignore own cell.
            if (x == p.x() && y == p.y()) {
              continue;
            }

            out.push_back(Coordinates(x, y));
          }
        }

        return out;
      }
    }

    /// @brief - None related functions.
    namespace none {

      inline
      std::vector<Coordinates>
      threaten(const Coordinates& /*p*/,
               const Color& /*c*/,
               const Board& /*b*/) noexcept
      {
        // Nothing is threatened by a none piece.
        return std::vector<Coordinates>();
      }
    }

    std::vector<Coordinates>
    threaten(const Type& t,
             const Color& c,
             const Coordinates& p,
             const Board& b) noexcept
    {
      switch (t) {
        case Pawn:
          return pawn::threaten(p, c, b);
        case Knight:
          return knight::threaten(p, c, b);
        case Bishop:
          return bishop::threaten(p, c, b);
        case Rook:
          return rook::threaten(p, c, b);
        case Queen:
          return queen::threaten(p, c, b);
        case King:
          return king::threaten(p, c, b);
        case None:
        default:
          return none::threaten(p, c, b);
      }
    }

  }
}
