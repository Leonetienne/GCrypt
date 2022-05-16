#ifndef GENERALUTILITY_CONTAINERUTILITY_H
#define GENERALUTILITY_CONTAINERUTILITY_H

#include <algorithm>

namespace Leonetienne {
    namespace GeneralUtility {

        class ContainerUtility {
        public:
            //! Will return the index of `item` in `set`.
            //! \tparam T_Type The type of `item`
            //! \tparam T_Container The type of container
            //! \param item The item to find the index for
            //! \param set The container to be looking in
            //! \return The index of `item` in `set`. -1 if not found.
            template<typename T_Type, class T_Container>
            static int Ord(const T_Type &item, const T_Container &set);

        private:
            // No instantiation! >:(
            ContainerUtility();
        };

        template<typename T_Type, class T_Container>
        int ContainerUtility::Ord(const T_Type &item, const T_Container &set) {
            const auto result =
                    std::find_if(set.begin(), set.end(), [item](const T_Type &c) -> bool {
                        return c == item;
                    });

            // No item found
            if (result == set.end())
                return -1;
            else
                return result - set.begin();
        }
    }
}

#endif //GENERALUTILITY_CONTAINERUTILITY_H
