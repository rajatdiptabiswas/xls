// Copyright 2020 The XLS Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "xls/ir/proc.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "xls/common/status/matchers.h"
#include "xls/ir/ir_matcher.h"
#include "xls/ir/ir_test_base.h"

namespace m = ::xls::op_matchers;

namespace xls {
namespace {

class ProcTest : public IrTestBase {};

TEST_F(ProcTest, SimpleProc) {
  auto p = CreatePackage();
  XLS_ASSERT_OK_AND_ASSIGN(Proc * proc, ParseProc(R"(
proc foo(in_token: token, in_state: bits[32], init=42) {
  literal.1: bits[32] = literal(value=1)
  add.2: bits[32] = add(literal.1, in_state)
  ret tuple.3: (token, bits[32]) = tuple(in_token, add.2)
}
)",
                                                  p.get()));
  EXPECT_EQ(proc->name(), "foo");
  EXPECT_EQ(proc->node_count(), 5);
  EXPECT_EQ(proc->return_value()->op(), Op::kTuple);

  EXPECT_THAT(proc->return_value(),
              m::Tuple(m::Param("in_token"),
                       m::Add(m::Literal(1), m::Param("in_state"))));
}

}  // namespace
}  // namespace xls
