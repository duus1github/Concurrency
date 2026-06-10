#include <string_view>
#include <initializer_list>
#define BS_THREAD_POOL_INIT_FUNC_CONCEPT(F) typename F, typename = std::enable_if_t<std::is_invocable_v<F> || std::is_invocable_v<F, std::size_t>> // NOLINT(bugprone-macro-parentheses)


namespace{
enum class tp : std::uint8_t
{
    none = 0,
    priority = 1 << 0,
    pause = 1 << 1,
    wait_deadlock_checks = 1 << 2

};

template <tp OptFlags = tp::none>
class [[nodiscard]] thread_pool{
public:
    static constexpr bool priority_enable = (OptFlags & tp::priority) != tp::none;
    static constexpr bool pause_enable = (OptFlags & tp::pause) != tp::none;
    static constexpr bool pause_enabled = (OptFlags & tp::pause) != tp::none;
    static constexpr bool wait_deadlock_checks_enabled = (OptFlags & tp::wait_deadlock_checks) != tp::none;

    thread_pool():thread_pool(0,[]){}
    explicit thread_pool(const std::size_t num_threads):thread_pool(num_threads,[]){}

    template<BS_THREAD_POOL_INIT_FUNC_CONCEPT(F)>
    explicit thread_pool(F&& init):thread_pool(0,std::forward<F>(init)){}

    template<BS_THREAD_POOL_INIT_FUNC_CONCEPT(F)>
    thread_pool(const std::size_t num_threads,F&& init){
        create_threads(num_threads, std::forward<F>(init));
    }
};
}