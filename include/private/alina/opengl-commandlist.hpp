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
        struct WriteBuffer {
            Buffer* buffer; 
            const void* data;
            size_t size;
            size_t offset;
        };
        struct ClearBuffer {
            Buffer* buffer;
            uint32_t clearValue;
        };
        struct BindGraphicsPipeline {
            alina::GraphicsPipeline* pipeline;
        };
    };
    class CommandList : public ::alina::CommandList {
    public:
        using Command = std::variant<
            Commands::Draw,
            Commands::DrawIndexed,
            Commands::WriteBuffer,
            Commands::ClearBuffer,
            Commands::BindGraphicsPipeline
        >;
        void begin() override;
        void bindGraphicsPipeline(alina::GraphicsPipeline* pipeline) override;
        void draw(const DrawArguments& drawArgs) override;
        void drawIndexed(const DrawArguments& drawArgs) override;
        void writeBuffer(Buffer* buffer, const void* data, size_t size, size_t offset) override;
        void clearBuffer(Buffer* buffer, uint32_t clearValue) override;
        void end() override;
        std::vector<Command> commands = {};
        bool doneRecording = false;
    };
}