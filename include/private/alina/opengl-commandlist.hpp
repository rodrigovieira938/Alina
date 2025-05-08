#pragma once
#include <alina/alina.hpp>
#include <variant>
#include <vector>

namespace alina::opengl {
    namespace Commands {
        struct Draw {
            DrawArguments args;
        };
        struct DrawIndexed {
            DrawArguments args;
        };
    };
    class CommandList : public ::alina::CommandList {
    public:
        using Command = std::variant<
            Commands::Draw,
            Commands::DrawIndexed
        >;
        void begin() override;
        void draw(const DrawArguments& drawArgs) override;
        void drawIndexed(const DrawArguments& drawArgs) override;
        void end() override;
        std::vector<Command> commands = {};
        bool doneRecording = false;
    };
}