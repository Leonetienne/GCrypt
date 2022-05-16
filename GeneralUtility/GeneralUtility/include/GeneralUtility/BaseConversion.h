#ifndef GENERALUTILITY_BASECONVERSION_H
#define GENERALUTILITY_BASECONVERSION_H

#include <algorithm>
#include <utility>
#include <sstream>
#include <stdexcept>

#include "ContainerUtility.h"

namespace Leonetienne {
    namespace GeneralUtility {

        class BaseConversion {
        public:
            //! Will divide a number of arbitrary base in `dividend` by an integer divisor.
            //! This is a specific helper function for the base conversion functions.
            //! \tparam T_Container The type of container used for the digitstring
            //! \param dividend The number to be divided in listlike container-form (such as a string)
            //! \param divisor The integer divisor
            //! \param set The set/base of `dividend`
            //! \return A pair of the result. (result, rest)
            template<class T_Container>
            static std::pair<T_Container, int>
            DigitstringDivision(const T_Container &dividend, const unsigned int divisor, const T_Container &set);

            //! Will convert a number of arbitrary base to base 10
            //! \tparam T_Container The type of container used for the digitstring
            //! \param num A listlike container representing the number (such as a string)
            //! \param set The set/base of the number
            //! \return A 64-bit integer representing the number
            template<class T_Container>
            static std::uint64_t BaseX_2_10(const T_Container &num, const T_Container &set);

            //! Will convert a number to an arbitrary base.
            //! This just a wrapper for BaseX_2_Y.
            //! \tparam T_Container The type of container used for the digitstring
            //! \param num The number to be converted
            //! \param set The desired set/base for the output to be in. Should be a listlike container (such as a string)
            //! \param minOutLen The minimum output length. Setting this will result in zero-padded output (Like, 00000001 instead of 1)
            //! \return `num` in base `set`
            template<class T_Container>
            static T_Container
            Base10_2_X(const std::uint64_t &num, const T_Container &set, const std::uint32_t minOutLen = 1);

            //! Will convert a number from an arbitrary base to another arbitrary base.
            //! \tparam T_ContainerIn The type of container used for the incoming digitstring
            //! \tparam T_ContainerOut The type of container used for the outgoing digitstring
            //! \param num A representation of a number in a listlike container (such as a string)
            //! \param set_in The set/base of the input
            //! \param set_out The desired set/base to output
            //! \param minOutLen The minimum output length. Setting this will result in zero-padded output (Like, 00000001 instead of 1)
            //! \return `num` in base `set_out`
            template<class T_ContainerIn, class T_ContainerOut>
            static T_ContainerOut
            BaseX_2_Y(const T_ContainerIn &num, const T_ContainerIn &set_in, const T_ContainerOut &set_out,
                      const std::uint32_t minOutLen = 1);

        private:
            // No instantiation! >:(
            BaseConversion();
        };

        namespace {
            // Fast 64-bit int power function
            inline std::uint64_t Powuli(const std::uint64_t &b, const std::uint64_t &e) {
                std::uint64_t buf = 1;

                for (std::uint64_t i = 0; i < e; i++)
                    buf *= b;

                return buf;
            }
        }

        template<class T_Container>
        std::uint64_t BaseConversion::BaseX_2_10(const T_Container &num, const T_Container &set) {
            // If base is 0, throw logic error
            if (set.size() == 0)
                throw std::logic_error("Can't convert from base0! Please supply a nonempty set!");

            unsigned long long int buf = 0;
            for (std::size_t i = 0; i < num.size(); i++) {
                for (std::size_t j = 0; j < set.size(); j++) {
                    if (num[i] == set[j]) {
                        buf += Powuli((std::uint64_t) set.size(), (uint64_t) (num.size() - (i + 1))) * j;
                        break;
                    }
                }
            }

            return buf;
        }

// Based on: https://www.geeksforgeeks.org/divide-large-number-represented-string/
        template<class T_Container>
        std::pair<T_Container, int>
        BaseConversion::DigitstringDivision(const T_Container &dividend, const unsigned int divisor,
                                            const T_Container &set) {
            // Quick rejects:

            // No set? Throw logic error
            if (set.size() == 0)
                throw std::logic_error("Can't divide a number of base0! Please supply a nonempty set!");

            // No division by 0
            if (divisor == 0)
                throw std::overflow_error("Division by zero!");

            // Dividend size 0? Return 0.
            if (dividend.size() == 0)
                return std::make_pair(T_Container({set[0]}), 0);

            // Verify that all digits are represented in the set/base
            for (const auto &c: dividend)
                if (ContainerUtility::Ord(c, set) == -1)
                    throw std::logic_error(
                            "The supplied dividend contains a digit that is not represented in the set/base!");


            // Now for the actual algorithm:
            T_Container result;

            // Find prefix of number that is larger than divisor.
            int idx = 0;
            int temp = ContainerUtility::Ord(dividend[idx], set);
            while (temp < divisor) {
                idx++;
                if (idx < dividend.size())
                    temp = temp * set.size() + ContainerUtility::Ord(dividend[idx], set);
                else
                    break;
            }

            // Repeatedly divide divisor with temp. After
            // every division, update temp to include one
            // more digit.
            int curRest = temp % divisor;
            while (dividend.size() > idx) {
                // Store result in answer i.e. temp / divisor
                result.insert(result.cend(), set[temp / divisor]);
                curRest = temp % divisor;

                // Take next digit of number
                idx++;
                if (idx < dividend.size())
                    temp = (temp % divisor) * set.size() + ContainerUtility::Ord(dividend[idx], set);
            }

            // If divisor is greater than number
            if (result.size() == 0) {
                // Generate 0-value digitstring
                result.clear();
                result.insert(result.cend(), set[0]);
                return std::make_pair(result, BaseX_2_10(dividend, set));
            }

            // else return the answer
            return std::make_pair(result, curRest);
        }

        template<class T_ContainerIn, class T_ContainerOut>
        T_ContainerOut
        BaseConversion::BaseX_2_Y(const T_ContainerIn &num, const T_ContainerIn &set_in, const T_ContainerOut &set_out,
                                  const std::uint32_t minOutLen) {
            if ((set_in.size() == 0) || (set_out.size() == 0))
                throw std::logic_error("Can't convert from or to base0! Please supply a nonempty set!");

            T_ContainerOut result;

            // Generate a 0-value string for inbase
            const T_ContainerIn zeroInbase({set_in[0]});

            if (num != zeroInbase) {
                // Populate result object
                {
                    T_ContainerIn buf = num;
                    while (buf != zeroInbase) {
                        const auto divRes = DigitstringDivision(buf, set_out.size(), set_in);
                        const std::uint64_t mod = divRes.second;
                        buf = divRes.first;
                        result.insert(result.cend(), set_out[mod]);
                    }
                }

                // Reverse result object item order
                {
                    // Now reverse result
                    T_ContainerOut buf = result;
                    result.clear();
                    for (std::size_t i = 0; i < buf.size(); i++)
                        result.insert(result.cend(), buf[buf.size() - i - 1]);
                }
            } else {
                // If num is 0, just pass a null-value. The algorithm would hang otherwise.
                result.insert(result.cend(), set_out[0]);
            }


            // Add as much null-values to the left as requested.
            if (result.size() < minOutLen) {
                const std::size_t cachedLen = result.size();
                const T_ContainerOut cachedStr = result;
                result.clear();
                for (std::size_t i = 0; i < minOutLen - cachedLen; i++)
                    result.insert(result.cend(), set_out[0]);

                for (const auto &it: cachedStr)
                    result.insert(result.cend(), it);
            }

            return result;
        }

        template<class T_Container>
        T_Container
        BaseConversion::Base10_2_X(const std::uint64_t &num, const T_Container &set, const std::uint32_t minOutLen) {
            // Convert num to a string
            std::stringstream ss;
            ss << num;
            const std::string numStr = ss.str();

            // Use BaseX_2_Y to convert to outbase
            const T_Container convertedNum = BaseX_2_Y<std::string, T_Container>(numStr, "0123456789", set, minOutLen);

            // return it
            return convertedNum;
        }


    }
}

#endif //GENERALUTILITY_BASECONVERSION_H
