#pragma once
#include <filesystem>
#include <optional>
#include <vector>

namespace kslicer {

    class CommandLineParser {
    public:
        template <typename T>
        void add_option(std::string name, T& destination) {
            Option o{};
            o.name = name;
            o.store = [](void* destination, const std::filesystem::path& value) {
                CommandLineParser::store(*static_cast<T*>(destination), value);
            };
            o.destination = &destination;
            o.is_required = true;
            options_.push_back(o);
        }

        template <typename T>
        void add_option(std::string name, std::optional<T>& destination) {
            Option o{};
            o.name = name;
            o.store = [](void* destination, const std::filesystem::path& value) {
                CommandLineParser::store(static_cast<std::optional<T>*>(destination)->emplace(), value);
            };
            o.destination = &destination;
            o.is_required = false;
            options_.push_back(o);
        }

        void add_flag(std::string name, bool& destination) {
            Option o{};
            o.name = name;
            o.store = nullptr;
            o.destination = &destination;
            o.is_required = false;
            options_.push_back(o);
        }

        bool parse(const std::vector<std::filesystem::path>& args) const;

    private:
        static void store(std::filesystem::path& destination, const std::filesystem::path& value) {
            destination = value;
        }

        static void store(std::string& destination, const std::filesystem::path& value) {
            destination = value.u8string();
        }

        struct Option {
            std::string name;
            void (*store)(void*, const std::filesystem::path&) = nullptr;
            void* destination = nullptr;
            bool is_required = true;
        };

        std::vector<Option> options_;
    };

} // namespace kslicer
