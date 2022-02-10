
/**
 * @brief - Experiment around creating a board game (in this
 *          case for chess) and a limited AI.
 */

# include <core_utils/StdLogger.hh>
# include <core_utils/PrefixedLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <core_utils/CoreException.hh>
# include "AppDesc.hh"
# include "TopViewFrame.hh"
# include "App.hh"

/// TODO: Handle the load/save of games.
/// TODO: Welcome menu and choose side.
/// TODO: Compute stalemate.

/// @brief - Size of the tiles.
# define TILE_SIZE 170

int
main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger raw;
  raw.setLevel(utils::Level::Debug);
  utils::PrefixedLogger logger("chess", "main");
  utils::LoggerLocator::provide(&raw);

  try {
    logger.logMessage(utils::Level::Notice, "Starting application");

    pge::Viewport tViewport = pge::newViewport(olc::vf2d(-1.5f, -1.5f), olc::vf2d(10.0f, 10.0f));
    pge::Viewport pViewport = pge::newViewport(olc::vf2d(0.0f, 0.0f), olc::vf2d(768.0f, 768.0f));

    pge::CoordinateFrameShPtr cf = std::make_shared<pge::TopViewFrame>(
      tViewport,
      pViewport,
      olc::vi2d(TILE_SIZE, TILE_SIZE)
    );
    pge::AppDesc ad = pge::newDesc(olc::vi2d(768, 768), cf, "chess-app");
    ad.fixedFrame = true;
    chess::App demo(ad);

    demo.Start();
  }
  catch (const utils::CoreException& e) {
    logger.logError(utils::Level::Critical, "Caught internal exception while setting up application", e.what());
  }
  catch (const std::exception& e) {
    logger.logError(utils::Level::Critical, "Caught internal exception while setting up application", e.what());
  }
  catch (...) {
    logger.logMessage(utils::Level::Critical, "Unexpected error while setting up application");
  }

  return EXIT_SUCCESS;
}
