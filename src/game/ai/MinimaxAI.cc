
# include "MinimaxAI.hh"
# include "MoveGeneration.hh"

namespace {

  float
  pieceValue(const chess::Piece& p) noexcept {
    if (p.pawn()) {
      return 10.0f;
    }
    else if (p.knight()) {
      return 30.0f;
    }
    else if (p.bishop()) {
      return 30.0f;
    }
    else if (p.rook()) {
      return 50.0f;
    }
    else if (p.queen()) {
      return 90.0f;
    }
    else if (p.king()) {
      return 900.0f;
    }
    else {
      // No piece or invalid type, null value.
      return 0.0f;
    }
  }

  float
  evaluate(const chess::Color& c,
           const chess::Board& b) noexcept
  {
    float weight = 0.0f;

    // Traverse the board and evaluate it.
    for (int y = 0 ; y < b.h() ; ++y) {
      for (int x = 0 ; x < b.w() ; ++x) {
        const chess::Piece& p = b.at(x, y);
        float ev = pieceValue(p);

        weight += (p.color() == c ? ev : -ev);
      }
    }

    return weight;
  }

}

namespace chess {

  MinimaxAI::MinimaxAI(const Color& color,
                       unsigned depth):
    AI(color, "minimax"),
    m_depth(depth)
  {}

  std::vector<ai::Move>
  MinimaxAI::generateMoves(const Board& b) noexcept {
    // Generate moves.
    std::vector<ai::Move> moves = ai::generate(m_color, b);

    // For each available position, evaluate the
    // state of the board after making the move.
    for (unsigned id = 0u ; id < moves.size() ; ++id) {
      /// TODO: Handle the move.
      moves[id].weight = evaluate(m_color, b);
    }

    return moves;

  }

}
