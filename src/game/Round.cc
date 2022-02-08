
# include "Round.hh"

namespace chess {

  Round::Round(unsigned id):
    m_id(id),

    m_valid(false),
    m_white({Piece::generate(), Coordinates(0, 0), Coordinates(0, 0), false, false, false}),
    m_black({Piece::generate(), Coordinates(0, 0), Coordinates(0, 0), false, false, false}),

    m_name()
  {
    generateName();
  }

  void
  Round::registerMove(PieceShPtr p,
                      const Coordinates& start,
                      const Coordinates& end,
                      bool captured,
                      bool check,
                      bool checkmate) noexcept
  {
    Part* out = &m_white;
    if (p->color() == Color::Black) {
      out = &m_black;
    }

    out->piece = p;

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
  Round::valid() const noexcept {
    return m_white.piece->valid() && m_black.piece->valid();
  }

  unsigned
  Round::id() const noexcept {
    return m_id;
  }

  std::string
  Round::toString() const noexcept {
    return m_name;
  }

  void
  Round::generateName() noexcept {
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
      std::string out = p.piece->algebraic();

      if (p.captured) {
        if (p.piece->pawn()) {
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
    if (m_black.piece->invalid()) {
      return;
    }

    m_name += " ";
    m_name += nameMove(m_black);

    if (m_black.checkmate) {
      m_name += " 0-1";
    }
  }

}
