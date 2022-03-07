
/**
 * @brief - Defines an application for an open-world cell
 *          game with discrete, grid-like coordinates.
 */

# include <core_utils/StdLogger.hh>
# include <core_utils/PrefixedLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <core_utils/CoreException.hh>
# include "AppDesc.hh"
# include "TopViewFrame.hh"
# include "App.hh"

/// TODO: The simulation speed should be handled through a custom
/// time provided by the `Game` to the `World` or by the `World`
/// to the `Grid` and elements. This should not only be an elapsed
/// time since the last frame.

int
main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger raw;
  raw.setLevel(utils::Level::Debug);
  utils::PrefixedLogger logger("pge", "main");
  utils::LoggerLocator::provide(&raw);

  try {
    logger.logMessage(utils::Level::Notice, "Starting application");

    pge::Viewport tViewport = pge::Viewport(olc::vf2d(-11.0f, -12.0f), olc::vf2d(30.0f, 22.5f));
    pge::Viewport pViewport = pge::Viewport(olc::vf2d(0.0f, 0.0f), olc::vf2d(800.0f, 600.0f));

    pge::CoordinateFrameShPtr cf = std::make_shared<pge::TopViewFrame>(
      tViewport,
      pViewport,
      olc::vi2d(64, 64)
    );
    pge::AppDesc ad = pge::newDesc(olc::vi2d(800, 600), cf, "cellify");
    pge::App demo(ad);

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
