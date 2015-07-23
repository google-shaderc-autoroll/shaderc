// Copyright 2015 The Shaderc Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Some of the tests here check code paths that are not checked by
// integration tests.
// Generally, these would be conditions not generated by the Glslang
// compiler.  It's easier to write these unit tests than to inject
// a dependency on a fake compiler.
#include "libshaderc_util/message.h"

#include <gtest/gtest.h>

using shaderc_util::MessageType;
using shaderc_util::ParseGlslangOutput;
using shaderc_util::string_piece;

namespace {

TEST(ParseGlslangOutputTest, EmptyMessageBody) {
  string_piece segment_number;
  string_piece line_number;
  string_piece rest;
  EXPECT_EQ(MessageType::Unknown,
            ParseGlslangOutput("WARNING: ", false, false, &segment_number,
                               &line_number, &rest));
  EXPECT_EQ(MessageType::Unknown,
            ParseGlslangOutput("ERROR: ", false, false, &segment_number,
                               &line_number, &rest));
}

TEST(ParseGlslangOutputTest, GlobalError) {
  string_piece segment_number;
  string_piece line_number;
  string_piece rest;
  EXPECT_EQ(
      MessageType::GlobalError,
      ParseGlslangOutput("ERROR: too many functions: got 1666473 of them",
                         false, false, &segment_number, &line_number, &rest));
  EXPECT_EQ("too many functions: got 1666473 of them", rest.str());

  EXPECT_EQ(
      MessageType::GlobalError,
      ParseGlslangOutput(
          "ERROR: #version: versions before 150 do not allow a profile token",
          false, false, &segment_number, &line_number, &rest));
  EXPECT_EQ("#version: versions before 150 do not allow a profile token",
            rest.str());
}

TEST(ParseGlslangOutputTest, GlobalWarning) {
  string_piece segment_number;
  string_piece line_number;
  string_piece rest;
  EXPECT_EQ(MessageType::GlobalWarning,
            ParseGlslangOutput("Warning, version 1000 is unknown.", false,
                               false, &segment_number, &line_number, &rest));
  EXPECT_EQ("version 1000 is unknown.", rest.str());
}

TEST(ParseGlslangOutputTest, InvalidSuffixAfterSegmentNumber) {
  string_piece segment_number;
  string_piece line_number;
  string_piece rest;
  EXPECT_EQ(MessageType::GlobalWarning,
            ParseGlslangOutput("WARNING: 12a", false, false, &segment_number,
                               &line_number, &rest));
  EXPECT_EQ(MessageType::GlobalError,
            ParseGlslangOutput("WARNING: 12a", true, false, &segment_number,
                               &line_number, &rest));
  EXPECT_EQ(MessageType::GlobalError,
            ParseGlslangOutput("ERROR: 42!", false, false, &segment_number,
                               &line_number, &rest));
}

TEST(ParseGlslangOutputTest, OnlyANumber) {
  string_piece source_name;
  string_piece line_number;
  string_piece rest;
  EXPECT_EQ(MessageType::GlobalWarning,
            ParseGlslangOutput("WARNING: 12", false, false, &source_name,
                               &line_number, &rest));
  EXPECT_TRUE(source_name.empty());
  EXPECT_TRUE(line_number.empty());
  EXPECT_EQ("12", rest.str());

  EXPECT_EQ(MessageType::GlobalError,
            ParseGlslangOutput("WARNING: 12", true, false, &source_name,
                               &line_number, &rest));
  EXPECT_TRUE(source_name.empty());
  EXPECT_TRUE(line_number.empty());
  EXPECT_EQ("12", rest.str());

  EXPECT_EQ(MessageType::GlobalError,
            ParseGlslangOutput("ERROR: 42", false, false, &source_name,
                               &line_number, &rest));
  EXPECT_TRUE(source_name.empty());
  EXPECT_TRUE(line_number.empty());
  EXPECT_EQ("42", rest.str());
}

TEST(ParseGlslangOutputTest, InvalidSuffixAfterSegmentNumberColon) {
  string_piece segment_number;
  string_piece line_number;
  string_piece rest;
  EXPECT_EQ(MessageType::GlobalWarning,
            ParseGlslangOutput("WARNING: 12:0", false, false, &segment_number,
                               &line_number, &rest));
  EXPECT_EQ(MessageType::GlobalError,
            ParseGlslangOutput("ERROR: 42:1234", false, false, &segment_number,
                               &line_number, &rest));
}

TEST(ParseGlslangOutputTest, CompletelyUnrecognized) {
  string_piece segment_number;
  string_piece line_number;
  string_piece rest;
  EXPECT_EQ(MessageType::Unknown,
            ParseGlslangOutput("hello world!", false, false, &segment_number,
                               &line_number, &rest));
}

TEST(ParseGlslangOutputTest, LocationSpecification) {
  string_piece segment_number;
  string_piece line_number;
  string_piece rest;

  EXPECT_EQ(
      MessageType::Error,
      ParseGlslangOutput("ERROR: 0:2: '#' : invalid directive: foo", false,
                         false, &segment_number, &line_number, &rest));
  EXPECT_EQ("0", segment_number.str());
  EXPECT_EQ("2", line_number.str());
  EXPECT_EQ("'#' : invalid directive: foo", rest.str());

  EXPECT_EQ(MessageType::Warning,
            ParseGlslangOutput(
                "WARNING: 15:36: The following extension must be "
                "enabled to use this feature:",
                false, false, &segment_number, &line_number, &rest));
  EXPECT_EQ("15", segment_number.str());
  EXPECT_EQ("36", line_number.str());
  EXPECT_EQ("The following extension must be enabled to use this feature:",
            rest.str());
}

TEST(ParseGlslangOutputTest, FileName) {
  string_piece source_name;
  string_piece line_number;
  string_piece rest;

  EXPECT_EQ(MessageType::Error,
            ParseGlslangOutput("ERROR: shader.vert:5: something wrong", false,
                               false, &source_name, &line_number, &rest));
  EXPECT_EQ("shader.vert", source_name.str());
  EXPECT_EQ("5", line_number.str());
  EXPECT_EQ("something wrong", rest.str());

  EXPECT_EQ(MessageType::Warning,
            ParseGlslangOutput("WARNING: file:42: something wrong", false,
                               false, &source_name, &line_number, &rest));
  EXPECT_EQ("file", source_name.str());
  EXPECT_EQ("42", line_number.str());
  EXPECT_EQ("something wrong", rest.str());

  EXPECT_EQ(MessageType::Warning,
            ParseGlslangOutput("WARNING: 0xdeedbeef:0: wa:ha:ha", false, false,
                               &source_name, &line_number, &rest));
  EXPECT_EQ("0xdeedbeef", source_name.str());
  EXPECT_EQ("0", line_number.str());
  EXPECT_EQ("wa:ha:ha", rest.str());
}

}  // anonymous namespace
