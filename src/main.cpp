#include <logger.hpp>

auto main() -> int
{
    makedump::logger logger { makedump::logger::format("{white+}", ">>>") };
    logger.print("{green+}", "Heir To Empire");
}
