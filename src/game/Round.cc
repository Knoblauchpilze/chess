
# include "Round.hh"

namespace chess {

  Round::Round(unsigned id):
    utils::CoreObject("round"),

    m_id(id),

    m_valid(false),
    m_white({Piece::generate(), Coordinates(), Coordinates(), false, false, false, false}),
    m_black({Piece::generate(), Coordinates(), Coordinates(), false, false, false, false}),

    m_name()
  {
    setService("chess");
    generateName();
  }

  void
  Round::registerMove(Piece p,
                      const Coordinates& start,
                      const Coordinates& end,
                      bool captured,
                      bool check,
                      bool checkmate,
                      bool stalemate) noexcept
  {
    Part* out = &m_white;
    if (p.color() == Color::Black) {
      out = &m_black;
    }

    out->piece = p;

    out->start = start;
    out->end = end;

    out->captured = captured;

    out->check = check;
    out->checkmate = checkmate;
    out->stalemate = stalemate;

    generateName();
  }

  bool
  Round::whitePlayed() const noexcept {
    return m_white.piece.valid();
  }

  bool
  Round::blackPlayed() const noexcept {
    return m_black.piece.valid();
  }

  bool
  Round::valid() const noexcept {
    return whitePlayed() && blackPlayed();
  }

  unsigned
  Round::id() const noexcept {
    return m_id;
  }

  Coordinates
  Round::getMoveStart(const Color& c) const {
    if (c == Color::White) {
      if (!whitePlayed()) {
        error(
          "Failed to get starting position for move",
          "White move is not registered"
        );
      }

      return m_white.start;
    }

    // Assume we request the black color.
    if (!blackPlayed()) {
      error(
        "Failed to get starting position for move",
        "Black move is not registered"
      );
    }

    return m_black.start;
  }

  Coordinates
  Round::getMoveEnd(const Color& c) const {
    if (c == Color::White) {
      if (!whitePlayed()) {
        error(
          "Failed to get ending position for move",
          "White move is not registered"
        );
      }

      return m_white.end;
    }

    // Assume we request the black color.
    if (!blackPlayed()) {
      error(
        "Failed to get ending position for move",
        "Black move is not registered"
      );
    }

    return m_black.end;
  }

  std::string
  Round::toString() const noexcept {
    return m_name;
  }

  void
  Round::generateName() noexcept {
    // Always include the identifier.
    m_name = std::to_string(m_id + 1);
    m_name += ".";

    // In case of an invalid move, generate an invalid
    // string.
    if (!valid()) {
      // Case of checkmate.
      if (!m_white.checkmate && !m_white.stalemate) {
        m_name += " N/A";
        return;
      }
    }

    auto nameMove = [](const Part& p) {
      std::string out;

      // Handle castling.
      if (p.piece.king() && std::abs(p.start.x() - p.end.x()) > 1) {
        if (p.start.x() > p.end.x()) {
          out += "0-0-0";
        }
        else {
          out += "0-0";
        }

        return out;
      }

      out = p.piece.algebraic();

      if (p.captured) {
        if (p.piece.pawn()) {
          out += cells::file(p.start.asValue());
        }
        out += "x";
      }
      out += cells::toString(p.end.asValue());

      if (p.piece.pawn() && !p.captured && p.start.x() != p.end.x()) {
        out += " e.p.";
      }

      if (p.checkmate) {
        out += "#";
      }
      else if (p.check) {
        out += "+";
      }

      return out;
    };

    // Generate the name from white's move.
    m_name += nameMove(m_white);

    if (m_white.checkmate) {
      m_name += " 1-0";
    }

    // Generate the name from black's move if needed.
    if (m_black.piece.invalid()) {
      return;
    }

    m_name += " ";
    m_name += nameMove(m_black);

    if (m_black.checkmate) {
      m_name += " 0-1";
    }
  }

}
