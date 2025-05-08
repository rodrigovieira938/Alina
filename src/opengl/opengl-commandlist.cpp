#include <alina/opengl-commandlist.hpp>

namespace alina::opengl {
    void CommandList::begin() {
        doneRecording = false;
        commands.clear();
    }
    void CommandList::draw(const DrawArguments& drawArgs) {
        commands.emplace_back(
            Commands::Draw {
                drawArgs
            }
        );
    }
    void CommandList::drawIndexed(const DrawArguments& drawArgs) {
        commands.emplace_back(
            Commands::DrawIndexed {
                drawArgs
            }
        );
    }
    void CommandList::end() {
        doneRecording = true;
    }
}