//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-14 17:04:45.
// File name: entity_enumerator.cc
//
// Description:
// Define class EntityEnumberator.
//

#include "game_server/server/extension/scene/entity_enumerator.h"

#include "entity/camp_types.h"
#include "entity/npc_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/scene/facade_request.h"
#include "game_server/server/extension/scene/grid.h"
#include "game_server/server/extension/scene/scene.h"
#include "global/area_point_define.h"
#include "global/logging.h"

using std::make_pair;

namespace game {

namespace server {

namespace scene {

bool EntityFilter::Initialize() {
  // (360 ~ 315]
  // this->dir_indexs_.insert(std::make_pair(360, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(359, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(358, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(357, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(356, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(355, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(354, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(353, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(352, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(351, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(350, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(349, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(348, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(347, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(346, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(345, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(344, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(343, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(342, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(341, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(340, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(339, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(338, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(337, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(336, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(335, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(334, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(333, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(332, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(331, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(330, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(329, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(328, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(327, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(326, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(325, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(324, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(323, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(322, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(321, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(320, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(319, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(318, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(317, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(316, (size_t)global::RayArrayIndex::INDEX_22));
  this->dir_indexs_.insert(std::make_pair(315, (size_t)global::RayArrayIndex::INDEX_22));

  // (315 ~ 270]
  this->dir_indexs_.insert(std::make_pair(314, (size_t)global::RayArrayIndex::INDEX_22));
  this->dir_indexs_.insert(std::make_pair(313, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(312, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(311, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(310, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(309, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(308, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(307, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(306, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(305, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(304, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(303, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(302, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(301, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(300, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(299, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(298, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(297, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(296, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(295, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(294, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(293, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(292, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(291, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(290, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(289, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(288, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(287, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(286, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(285, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(284, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(283, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(282, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(281, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(280, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(279, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(278, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(277, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(276, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(275, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(274, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(273, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(272, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(271, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(270, (size_t)global::RayArrayIndex::INDEX_1));

  // (270 ~ 225]
  this->dir_indexs_.insert(std::make_pair(269, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(268, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(267, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(266, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(265, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(264, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(263, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(262, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(261, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(260, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(259, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(258, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(257, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(256, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(255, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(254, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(253, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(252, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(251, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(250, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(249, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(248, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(247, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(246, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(245, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(244, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(243, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(242, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(241, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(240, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(239, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(238, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(237, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(236, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(235, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(234, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(233, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(232, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(231, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(230, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(229, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(228, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(227, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(226, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(225, (size_t)global::RayArrayIndex::INDEX_22));

  // (225, 180]
  this->dir_indexs_.insert(std::make_pair(224, (size_t)global::RayArrayIndex::INDEX_22));
  this->dir_indexs_.insert(std::make_pair(223, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(222, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(221, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(220, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(219, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(218, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(217, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(216, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(215, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(214, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(213, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(212, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(211, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(210, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(209, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(208, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(207, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(206, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(205, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(204, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(203, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(202, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(201, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(200, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(199, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(198, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(197, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(196, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(195, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(194, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(193, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(192, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(191, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(190, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(189, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(188, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(187, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(186, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(185, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(184, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(183, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(182, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(181, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(180, (size_t)global::RayArrayIndex::INDEX_1));

  // (180 ~ 135]
  this->dir_indexs_.insert(std::make_pair(179, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(178, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(177, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(176, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(175, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(174, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(173, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(172, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(171, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(170, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(169, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(168, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(167, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(166, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(165, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(164, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(163, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(162, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(161, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(160, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(159, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(158, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(157, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(156, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(155, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(154, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(153, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(152, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(151, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(150, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(149, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(148, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(147, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(146, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(145, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(144, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(143, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(142, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(141, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(140, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(139, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(138, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(137, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(136, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(135, (size_t)global::RayArrayIndex::INDEX_22));

  // (135 ~ 90]
  this->dir_indexs_.insert(std::make_pair(134, (size_t)global::RayArrayIndex::INDEX_22));
  this->dir_indexs_.insert(std::make_pair(133, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(132, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(131, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(130, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(129, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(128, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(127, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(126, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(125, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(124, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(123, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(122, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(121, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(120, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(119, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(118, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(117, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(116, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(115, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(114, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(113, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(112, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(111, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(110, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(109, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(108, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(107, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(106, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(105, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(104, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(103, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(102, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(101, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(100, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(99,  (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(98,  (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(97,  (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(96,  (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(95,  (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(94,  (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(93,  (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(92,  (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(91,  (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(90,  (size_t)global::RayArrayIndex::INDEX_1));

  // (89 ~ 45]
  this->dir_indexs_.insert(std::make_pair(89, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(88, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(87, (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(86, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(85, (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(84, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(83, (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(82, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(81, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(80, (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(79, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(78, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(77, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(76, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(75, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(74, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(73, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(72, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(71, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(70, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(69, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(68, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(67, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(66, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(65, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(64, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(63, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(62, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(61, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(60, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(59, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(58, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(57, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(56, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(55, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(54, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(53, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(52, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(51, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(50, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(49, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(48, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(47, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(46, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(45, (size_t)global::RayArrayIndex::INDEX_22));

  // (45 ~ 0]
  this->dir_indexs_.insert(std::make_pair(44, (size_t)global::RayArrayIndex::INDEX_22));
  this->dir_indexs_.insert(std::make_pair(43, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(42, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(41, (size_t)global::RayArrayIndex::INDEX_21));
  this->dir_indexs_.insert(std::make_pair(40, (size_t)global::RayArrayIndex::INDEX_20));
  this->dir_indexs_.insert(std::make_pair(39, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(38, (size_t)global::RayArrayIndex::INDEX_19));
  this->dir_indexs_.insert(std::make_pair(37, (size_t)global::RayArrayIndex::INDEX_18));
  this->dir_indexs_.insert(std::make_pair(36, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(35, (size_t)global::RayArrayIndex::INDEX_17));
  this->dir_indexs_.insert(std::make_pair(34, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(33, (size_t)global::RayArrayIndex::INDEX_16));
  this->dir_indexs_.insert(std::make_pair(32, (size_t)global::RayArrayIndex::INDEX_15));
  this->dir_indexs_.insert(std::make_pair(31, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(30, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(29, (size_t)global::RayArrayIndex::INDEX_14));
  this->dir_indexs_.insert(std::make_pair(28, (size_t)global::RayArrayIndex::INDEX_13));
  this->dir_indexs_.insert(std::make_pair(27, (size_t)global::RayArrayIndex::INDEX_12));
  this->dir_indexs_.insert(std::make_pair(26, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(25, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(24, (size_t)global::RayArrayIndex::INDEX_11));
  this->dir_indexs_.insert(std::make_pair(23, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(22, (size_t)global::RayArrayIndex::INDEX_10));
  this->dir_indexs_.insert(std::make_pair(21, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(20, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(19, (size_t)global::RayArrayIndex::INDEX_9));
  this->dir_indexs_.insert(std::make_pair(18, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(17, (size_t)global::RayArrayIndex::INDEX_8));
  this->dir_indexs_.insert(std::make_pair(16, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(15, (size_t)global::RayArrayIndex::INDEX_7));
  this->dir_indexs_.insert(std::make_pair(14, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(13, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(12, (size_t)global::RayArrayIndex::INDEX_6));
  this->dir_indexs_.insert(std::make_pair(11, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(10, (size_t)global::RayArrayIndex::INDEX_5));
  this->dir_indexs_.insert(std::make_pair(9,  (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(8,  (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(7,  (size_t)global::RayArrayIndex::INDEX_4));
  this->dir_indexs_.insert(std::make_pair(6,  (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(5,  (size_t)global::RayArrayIndex::INDEX_3));
  this->dir_indexs_.insert(std::make_pair(4,  (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(3,  (size_t)global::RayArrayIndex::INDEX_2));
  this->dir_indexs_.insert(std::make_pair(2,  (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(1,  (size_t)global::RayArrayIndex::INDEX_1));
  this->dir_indexs_.insert(std::make_pair(0,  (size_t)global::RayArrayIndex::INDEX_1));

  return true;
}

void EntityFilter::FileterGrid(const Grid *grid) {
  if(grid == NULL) {
    global::LogError("%s:%d (%s) 参数 grid 为空", 
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(this->filter_mask_ & request::SelectMaskType::ACTOR) {
    const Grid::EntityVector &actors = grid->GetActors();
    Grid::EntityVector::const_iterator iterator = actors.begin();
    for(; iterator != actors.end(); ++iterator) {
      this->AddEntity((GameRole *)*iterator);
    }
  }

  if(this->filter_mask_ & request::SelectMaskType::NPC) {
    const Grid::EntityVector &npcs = grid->GetNpcs();
    Grid::EntityVector::const_iterator iterator = npcs.begin();
    for(; iterator != npcs.end(); ++iterator) {
      this->AddEntity((GameRole *)*iterator);
    }
  }
}

void EntityFilter::AddEntity(GameRole *role) {
  if(role == NULL) {
    return ;
  }

  bool select = false;
  bool check = false;

  // 检测技能条件
  // 检测是否包括自己
  if(((request::SelectMaskType::SELF & this->filter_mask_) != 0)) {
    if(role == this->src_role_) {
      select = true;
    }
    check = true;
  }

  if(select == false && ((request::SelectMaskType::NOT_SELF &
          this->filter_mask_) != 0)) {
    if(role != this->src_role_) {
      select = true;
    }
    check = true;
  }
  if(select == false && check == true) {
    return ;
  }
  select = false;
  check = false;

  // 检测队伍
  if(role->GetType() == entity::EntityType::TYPE_ACTOR && 
      (this->src_role_->GetType() == entity::EntityType::TYPE_ACTOR)) {
    if(select == false && ((request::SelectMaskType::TEAM &
            this->filter_mask_) != 0)) {
      if(this->src_role_ == role) {
        select = true;
      } else {
        core::uint64 src_team = FacadeRequest::GetInstance()->GetTeamID(
            this->src_role_->GetID());
        core::uint64 dest_team = FacadeRequest::GetInstance()->GetTeamID(
            role->GetID());
        // 是否包括同队玩家
        if((request::SelectMaskType::TEAM & this->filter_mask_) != 0 &&
            src_team != 0 && src_team == dest_team) {
          select = true;
        }
      }
      check = true;
    }
    if(select == false && ((request::SelectMaskType::OTHER_TEAM &
            this->filter_mask_) != 0)) {
      core::uint64 src_team = FacadeRequest::GetInstance()->GetTeamID(
          this->src_role_->GetID());
      core::uint64 dest_team = FacadeRequest::GetInstance()->GetTeamID(
          role->GetID());
      // 是否包括非同队玩家
      if((request::SelectMaskType::OTHER_TEAM & this->filter_mask_) != 0 &&
          (src_team == 0 || src_team != dest_team)) {
        select = true;
      }
      check = true;
    }
    if(select == false && check == true) {
      return ;
    }
    select = false;
    check = false;
  } else {
    // 有非玩家的对象检测队伍时一定不是同队
    if(select == false && ((request::SelectMaskType::TEAM &
            this->filter_mask_) != 0)) {
      check = true;
      select = false;
    }
    if(select == false && ((request::SelectMaskType::OTHER_TEAM &
            this->filter_mask_) != 0)) {
      check = true;
      select = true;
    }
    if(select == false && check == true) {
      return ;
    }
    select = false;
    check = false;
  }

  // 检测对象类型
  if((request::SelectMaskType::NPC & this->filter_mask_) != 0) {
    if(role->GetType() == entity::EntityType::TYPE_NPC) {
      entity::NpcKind::type type = ((GameNpc *)role)->GetNpcType();
      if(type == entity::NpcKind::MONSTER ||
          type == entity::NpcKind::TRANSPORT) {
        select = true;
      }
    }
    check = true;
  }
  if(select == false && ((request::SelectMaskType::ACTOR & this->filter_mask_) != 0 &&
        role->GetType() == entity::EntityType::TYPE_ACTOR)) {
    select = true;
    check = true;
  }
  if(select == false && check == true) {
    return ;
  }
  select = false;
  check = false;

  // 是否包括活着的玩家
  if((request::SelectMaskType::ALIVE & this->filter_mask_) != 0) {
    check = true;
    if(role->GetBasicStatus() != entity::RoleBasicStatus::STATUS_DIE) {
      select = true;
    }
  }

  // 是否包括死亡玩家
  if(select == false && (request::SelectMaskType::DEAD & this->filter_mask_) != 0) {
    check = true;
    if(role->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
      select = true;
    }
  }
  if(select == false && check == true) {
    return ;
  }
  select = false;
  check = false;

  // 获取阵营
  entity::CampRelationType::type camp_relation = game::server::Configure::GetInstance()->GetCampRelation(
      this->src_role_->GetAttribute(entity::RoleAoiFields::CAMP),
      role->GetAttribute(entity::RoleAoiFields::CAMP));
  // 是否包括友方阵营
  if(((request::SelectMaskType::CAMP_FRIEND & this->filter_mask_) != 0)) {
    if(camp_relation == entity::CampRelationType::FRIENDLY) {
      select = true;
    }
    check = true;
  }
  // 是否包括敌方阵营
  if(select == false && ((request::SelectMaskType::CAMP_ENEMY & this->filter_mask_) != 0)) {
    if(camp_relation == entity::CampRelationType::HOSTLITY) {
      select = true;
    }
    check = true;
  }
  // 是否包括中立阵营
  if(select == false && ((request::SelectMaskType::CAMP_NEUTRAL & this->filter_mask_) != 0)) {
    if(camp_relation == entity::CampRelationType::NEUTRAL) {
      select = true;
    }
    check = true;
  }
  // 是否包括无阵营
  if(select == false && ((request::SelectMaskType::CAMP_NONE & this->filter_mask_) != 0)) {
    if(camp_relation == entity::CampRelationType::NONE) {
      select = true;
    }
    check = true;
  }
  if(select == false && check == true) {
    return;
  }

  // pk检测
  if(role->GetType() == entity::EntityType::TYPE_ACTOR && 
      this->src_role_->GetType() == entity::EntityType::TYPE_ACTOR && 
      (request::SelectMaskType::PK_CHECK & this->filter_mask_)) {
    if(this->scene_) {
      if(this->scene_->PKCheck((GameActor *)this->src_role_, (GameActor *)role) != 0)
        return ;
    }
  }

  EntityPair entity_pair(role->GetID(), role->GetType());
  if(this->filters_.find(entity_pair) == this->filters_.end()) {
    this->entities_.push_back(entity_pair);
    this->filters_.insert(entity_pair);
  }
}

RayEnumberator::RayEnumberator() {}

RayEnumberator::~RayEnumberator() {}

bool RayEnumberator::Initialize(core::uint8 length, core::uint8 width,
    core::uint16 direction) {
  if(direction >= 360) {
    global::LogError("%s:%d (%s) 参数 direction 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(length > 25) {
    global::LogError("%s:%d (%s) 参数 length 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->filter_->Clear();
  this->length_ = length;
  this->width_ = width;
  this->direction_ = direction;

  return true;
}

void RayEnumberator::Execute(Scene *scene, core::uint32 x, core::uint32 y) {
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 参数 scene 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  size_t index = this->filter_->GetDirectionIndex(this->direction_);
  if(index >= global::RayArrayIndex::INDEX_MAX) {
    global::LogError("%s:%d (%s) 获取方向(%u)下标失败",
        __FILE__, __LINE__, __FUNCTION__, this->direction_);
    return ;
  }

  const global::OffsetPoint *array = global::AreaPointDefine::kRayArrays_[index];
  if(array == NULL) {
    global::LogError("%s:%d (%s) 获取射线数组失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  size_t max_number = global::AreaPointDefine::kRayArraysSize_[index];

  core::uint32 calculate_x = 0, calculate_y = 0;

  size_t pos = 0;
  for(; pos < max_number && pos < this->length_; ++pos) {
    int offset = 0 - this->width_/2;
    for(; offset <= this->width_/2; ++offset) {
      if(this->direction_ >= 315 &&
          this->direction_ < 360) {
        // x,y
        calculate_x = x + array[pos].offet_x_;
        calculate_y = y + array[pos].offet_y_ + offset;
      } else if(this->direction_ >= 270 &&
          this->direction_ < 315) {
        // y,x
        calculate_x = x + array[pos].offet_y_ + offset;
        calculate_y = y + array[pos].offet_x_;
      } else if(this->direction_ >= 225 &&
          this->direction_ < 270) {
        // 0-y,x
        calculate_x = x - array[pos].offet_y_ - offset;
        calculate_y = y + array[pos].offet_x_;
      } else if(this->direction_ >= 180 &&
          this->direction_ < 225) {
        // 0-x,y
        calculate_x = x - array[pos].offet_x_;
        calculate_y = y + array[pos].offet_y_ + offset;
      } else if(this->direction_ >= 135 &&
          this->direction_ < 180) {
        // 0-x,0-y
        calculate_x = x - array[pos].offet_x_;
        calculate_y = y - array[pos].offet_y_ - offset;
      } else if(this->direction_ >= 90 &&
          this->direction_ < 135) {
        // 0-y,0-x
        calculate_x = x - array[pos].offet_y_ - offset;
        calculate_y = y - array[pos].offet_x_;
      } else if(this->direction_ >= 45 &&
          this->direction_ < 90) {
        // y,0-x
        calculate_x = x + array[pos].offet_y_ + offset;
        calculate_y = y - array[pos].offet_x_;
      } else if(this->direction_ >= 0 &&
          this->direction_ < 45) {
        // x,0-y
        calculate_x = x + array[pos].offet_x_;
        calculate_y = y - array[pos].offet_y_ - offset;
      }
      const Grid *grid = scene->GetGrid(calculate_x, calculate_y);
      if(grid) {
        this->filter_->FileterGrid(grid);
        if(this->filter_->GetEntitySize() >= this->max_count_) {
          break;
        }
      }
    }
  }
}

CircleEnumberator::CircleEnumberator() : radius_(0) {}
CircleEnumberator::~CircleEnumberator() {}

bool CircleEnumberator::Initialize(core::uint8 radius) {
  if(radius > 12) {
    global::LogError("%s:%d (%s) 参数 radius(%u) 错误",
        __FILE__, __LINE__, __FUNCTION__, radius);
    return false;
  }

  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->filter_->Clear();

  this->radius_  = radius;

  return true;
}

void CircleEnumberator::Execute(Scene *scene, core::uint32 x, core::uint32 y) {
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 参数 scene 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  core::uint32 calculate_x = 0, calculate_y = 0;

  size_t pos = 0;
  for(; pos <= this->radius_; ++pos) {
    const global::OffsetPoint *array = global::AreaPointDefine::kRingArrays_[pos];
    if(array == NULL) {
      continue;
    }
    size_t pos2 = 0;
    for(; pos2 < global::AreaPointDefine::kRingArraysSize_[pos]; ++pos2) {
      calculate_x = x + array[pos2].offet_x_;
      calculate_y = y + array[pos2].offet_y_;
      const Grid *grid = scene->GetGrid(calculate_x, calculate_y);
      if(grid) {
        this->filter_->FileterGrid(grid);
        if(this->filter_->GetEntitySize() >= this->max_count_) {
          break;
        }
      }
    }
  }
}

SectorQuarterEnumberator::SectorQuarterEnumberator() : radius_(0) {}
SectorQuarterEnumberator::~SectorQuarterEnumberator() {}

bool SectorQuarterEnumberator::Initialize(core::uint8 radius, entity::DirectionType::type direction) {
  if(radius <= 0 || radius > 12 || direction < entity::DirectionType::MIN ||
      direction >= entity::DirectionType::NONE) {
    global::LogError("%s:%d (%s) 参数 radius(%u)/direction(%d) 错误",
        __FILE__, __LINE__, __FUNCTION__, radius, direction);
    return false;
  }

  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->filter_->Clear();

  this->radius_  = radius;
  this->direction_ = direction;

  return true;
}

void SectorQuarterEnumberator::Execute(Scene *scene, core::uint32 x, core::uint32 y) {
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 参数 scene 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  core::uint32 calculate_x = 0, calculate_y = 0;

  global::AreaPointDefine::SectorQuarterArray arrays =
    global::AreaPointDefine::kSectorQuarterArrays_[this->radius_];
  if(arrays == NULL) {
    LOG_ERROR("获取 kSectorArrays_ 失败, radius(%d)",
        this->radius_);
    return;
  }

  size_t pos = 0;
  for(; pos < global::AreaPointDefine::kSectorQuarterArraysSize_[this->radius_][this->direction_]; ++pos) {
    calculate_x = x + (*arrays)[this->direction_][pos].offet_x_;
    calculate_y = y - (*arrays)[this->direction_][pos].offet_y_;
    const Grid *grid = scene->GetGrid(calculate_x, calculate_y);
    if(grid) {
      this->filter_->FileterGrid(grid);
      if(this->filter_->GetEntitySize() >= this->max_count_) {
        break;
      }
    }
  }
}

SectorHalfEnumberator::SectorHalfEnumberator() : radius_(0) {}
SectorHalfEnumberator::~SectorHalfEnumberator() {}

bool SectorHalfEnumberator::Initialize(core::uint8 radius, entity::DirectionType::type direction) {
  if(radius <= 0 || radius > 12 || direction < entity::DirectionType::MIN ||
      direction >= entity::DirectionType::NONE) {
    global::LogError("%s:%d (%s) 参数 radius(%u)/direction(%d) 错误",
        __FILE__, __LINE__, __FUNCTION__, radius, direction);
    return false;
  }

  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->filter_->Clear();

  this->radius_  = radius;
  this->direction_ = direction;

  return true;
}

void SectorHalfEnumberator::Execute(Scene *scene, core::uint32 x, core::uint32 y) {
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 参数 scene 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(this->filter_ == NULL) {
    global::LogError("%s:%d (%s) filter 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  core::uint32 calculate_x = 0, calculate_y = 0;

  global::AreaPointDefine::SectorHalfArray arrays =
    global::AreaPointDefine::kSectorHalfArrays_[this->radius_];
  if(arrays == NULL) {
    LOG_ERROR("获取 kSectorArrays_ 失败, radius(%d)",
        this->radius_);
    return;
  }

  size_t pos = 0;
  for(; pos < global::AreaPointDefine::kSectorHalfArraysSize_[this->radius_][this->direction_]; ++pos) {
    calculate_x = x + (*arrays)[this->direction_][pos].offet_x_;
    calculate_y = y - (*arrays)[this->direction_][pos].offet_y_;
    const Grid *grid = scene->GetGrid(calculate_x, calculate_y);
    if(grid) {
      this->filter_->FileterGrid(grid);
      if(this->filter_->GetEntitySize() >= this->max_count_) {
        break;
      }
    }
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

