#pragma once

#include <type_traits>

namespace handy
{
    template<typename function_t,
    typename _ = std::enable_if_t< 
        std::is_nothrow_move_constructible_v<function_t> 
        &&  std::is_invocable_v<function_t>
        >>
    class finally
    {
        public:
            template<typename fn_t>
            finally(fn_t && aFn) :
                mFunction(std::forward<fn_t>(aFn))
            {}

            finally(finally const &) = delete;
            finally & operator = (finally const&) = delete;

            finally(finally && aOther) noexcept(std::is_nothrow_move_assignable_v<function_t>):
                mFunction(std::move(aOther.mFunction)),
                mActive(aOther.mActive)
            {
                aOther.mActive = false;
            }

            void commit() noexcept
            {
                mActive = false;
            }

            ~finally()
            {
                if(mActive)
                    mFunction();
            }

        private:
            function_t  mFunction;
            bool        mActive { true };
    };

    template<typename fn_t>
    finally(fn_t &&) -> finally<std::decay_t<fn_t>>;

}
