-------------------------------------------------------------------------------
-- Copyright (c) 2004 TENGWU Entertainment All rights reserved.
-- filename: GodMiracleCpatureDefine.h
-- author: chdeng
-- actor:
-- data: 2010-01-25
-- last:
-- brief: ������ṹ����
-------------------------------------------------------------------------------
-- ������
ClanGodMiracleNum = 5
Minrecord 	 = 0
ActiveID = 100

MapID_test02 = 3618849499
MapID_FX	 = 3017298383
MapID_ZL	 = 3017299663
MapID_WS	 = 3017299919
MapID_WY	 = 3017299151
MapID_KL	 = 3017299407

--!\ brief: ��ͼ�Ƿ���������������
function IsInGodAreaMap(MapID)
	if MapID == 3017298383 or MapID == 3017299663 or MapID == 3017299919 or MapID == 3017299151 or MapID == 3017299407 then
		return	1
	else
		return 0
	end
end

-- �õĵ�ö��
GodEnum =  {Null=0, Npc1=1, Npc2=2, Npc3=3, Defend=1, Capture=2, TRUE=1,FALSE=0, StdScoresTick=150,
			SilverBuffID = 9998601, RedBuffID=9998701, GreenBuffID= 9998901, BlueBuffID=9998801, OpenEffect=9998501}

-- ���������񼣵Ľṹ��
ClanGodMiracle_Proto = {}
ClanGodMiracle_Proto[MapID_FX] 	   = { ClanGodMiracleID=1, NPCID1 = 4900509, NPCID2 = 4900510, NPCID3 = 4900511, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, GodAreaID = 6862}
ClanGodMiracle_Proto[MapID_ZL] 	   = { ClanGodMiracleID=2, NPCID1 = 4900512, NPCID2 = 4900513, NPCID3 = 4900514, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, GodAreaID = 10502}
ClanGodMiracle_Proto[MapID_WS] 	   = { ClanGodMiracleID=3, NPCID1 = 4900515, NPCID2 = 4900516, NPCID3 = 4900517, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, GodAreaID = 6047}
ClanGodMiracle_Proto[MapID_WY] 	   = { ClanGodMiracleID=4, NPCID1 = 4900518, NPCID2 = 4900519, NPCID3 = 4900520, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, GodAreaID = 6197}
ClanGodMiracle_Proto[MapID_KL] 	   = { ClanGodMiracleID=5, NPCID1 = 4900521, NPCID2 = 4900522, NPCID3 = 4900523, NPCIDObj1=0, NPCIDObj2=0, NPCIDObj3=0, GodAreaID = 9456}

ClanGodMiracle_Pos = {}
ClanGodMiracle_Pos[MapID_FX]	   = { x1=2027, y1=10812, z1=2419, x2=1951, y2=10900, z2=2572, x3=2113, y3=10964, z3=2641}
ClanGodMiracle_Pos[MapID_ZL]	   = { x1=1062, y1=13476, z1=1873, x2=1152, y2=13311, z2=1828, x3=1262, y3=12925, z3=1886}
ClanGodMiracle_Pos[MapID_WS]	   = { x1=1816, y1=24427, z1=2611, x2=1848, y2=24425, z2=2487, x3=1715, y3=24425, z3=2536}
ClanGodMiracle_Pos[MapID_WY]	   = { x1=1743, y1=3015,  z1=806,  x2=1949, y2=3095,  z2=854,  x3=1965, y3=3095,  z3=697}
ClanGodMiracle_Pos[MapID_KL]	   = { x1=1186, y1=8429,  z1=2647, x2=817,  y2=8130 , z2=2727, x3=1038, y3=7460,  z3=2702}

--  \breif: ͨ������ID�õ����Ǹ�����
function GetGodStackeIndex(MapID, NpcTypeID)
	if ClanGodMiracle_Proto[MapID].NPCID1 == NpcTypeID then
		return GodEnum.Npc1
	elseif ClanGodMiracle_Proto[MapID].NPCID2 == NpcTypeID then
		return GodEnum.Npc2
	elseif ClanGodMiracle_Proto[MapID].NPCID3 == NpcTypeID then
		return GodEnum.Npc3
	end
	return GodEnum.Null
end

-- \breif: �õ�����ID
function GetAreaID(MapID)
	return ClanGodMiracle_Proto[MapID].GodAreaID
end

-- \breif: �õ���ID(Ϊ�ϲ��ṩ)
function GetGodMiracleIDMinOne(MapID)
	return ClanGodMiracle_Proto[MapID].ClanGodMiracleID -1
end


-- �������ʱ��Ϣ
ClanGodMiracle_CurInfo = {}
ClanGodMiracle_CurInfo[1] = {MapID = MapID_FX,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 4900504, openFlag = 1}
ClanGodMiracle_CurInfo[2] = {MapID = MapID_ZL,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 4900505, openFlag = 1}
ClanGodMiracle_CurInfo[3] = {MapID = MapID_WS,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 4900506, openFlag = 1}
ClanGodMiracle_CurInfo[4] = {MapID = MapID_WY,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 4900507, openFlag = 1}
ClanGodMiracle_CurInfo[5] = {MapID = MapID_KL,	   DefendGuild=0, CaptureGuild=0, Npc1 = -1, Npc2=-1, Npc3=-1, DefendSocre = 0, CaptureScores=0, tick=0, InitFlag=0, GdMiracleID = 4900508, openFlag = 1}

-- �������
Capture_Result = {}
Capture_Result[1] = {MapID = MapID_FX, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 4900504}
Capture_Result[2] = {MapID = MapID_ZL, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 4900505}
Capture_Result[3] = {MapID = MapID_WS, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 4900506}
Capture_Result[4] = {MapID = MapID_WY, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 4900507}
Capture_Result[5] = {MapID = MapID_KL, DefendGuild=0, CaptureGuild=0, WinOrLoseFlag = 0, GdMiracleID = 4900508}

-- \breif: ����񼣵�������
function ClearGodMiracleResult()
	for k, v in pairs(Capture_Result) do
		Capture_Result[k].DefendGuild = 0
		Capture_Result[k].CaptureGuild = 0
		Capture_Result[k].WinOrLoseFlag = 0
	end
end

-- \breif: �õ����ر�־
function GetOpenFlag(MapID)
	return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].openFlag
end

-- \breif: ���ĳ��
function ClearGodMiracle(GodMiracleID)
	ClanGodMiracle_CurInfo[GodMiracleID].DefendGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].Npc1 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc2 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc3 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].DefendSocre 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureScores 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].tick 			= 0
end

-- \breif: ����������������Ϣ
function ResetGodMiracle(GodMiracleID)
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureGuild 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].Npc1 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc2 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].Npc3 			= -1
	ClanGodMiracle_CurInfo[GodMiracleID].DefendSocre 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].CaptureScores 	= 0
	ClanGodMiracle_CurInfo[GodMiracleID].tick 			= 0
end

-- \breif: �õ���ͼID
function GetCurInfoMapID(GodMiracleID)			return ClanGodMiracle_CurInfo[GodMiracleID].MapID end

-- \breif:  ��ʼ����
function SetInitFlag(MapID, inFlag)			ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].InitFlag = inFlag	end
function GetInitFlag(MapID)					return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].InitFlag	end

-- \breif: �õ������÷�������ID
function GetDefendGuildID(MapID) 				return   ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild 	 end
function SetDefendGuildID(MapID, inGuildID)	ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild = inGuildID end

-- \breif: �õ��������������ID
function GetCaptureGuildID(MapID) 				return   ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild   end
function SetCaptureGuildID(MapID, inGuildID)	ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild = inGuildID end

-- \breif: ���Ӻ�����TICK
function Gettick(MapID)						return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick	end
function SetTick(MapID, inTick)				ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick = inTick	end
function ModTick(MapID, inTick)				ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick = ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].tick + inTick  end

-- \breif: ���ú͵��׻���
--  param2: �ط��򹥷��ı�־
function GetScores(MapID, DOrCFlag)
	if DOrCFlag ==  GodEnum.Defend then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre
	elseif DOrCFlag ==  GodEnum.Capture then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores
	end

end

function SetScores(MapID, DOrCFlag, inScores)
	if DOrCFlag ==  GodEnum.Defend then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre = inScores
	elseif DOrCFlag == GodEnum.Capture then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores = inScores
	end
end

function ModScores(MapID, DOrCFlag, inScores)
	if DOrCFlag ==  GodEnum.Defend then
		ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre =  ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendSocre + inScores
	elseif DOrCFlag ==  GodEnum.Capture then
		ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores =  ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureScores + inScores
	end
end


-- \breif: ���ú͵õ�ռ���񼣵İ���ID
function GetCapGodStacke(MapID, NpcType)
	if NpcType == GodEnum.Npc1 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1
	elseif NpcType == GodEnum.Npc2 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2
	elseif NpcType == GodEnum.Npc3 then
		return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3
	end
end

function SetCapGodStacke(MapID, NpcType, GuildID)
	if NpcType == GodEnum.Npc1 then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1 = GuildID
	elseif NpcType == GodEnum.Npc2 then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2 = GuildID
	elseif NpcType == GodEnum.Npc3 then
		 ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3 = GuildID
	end
end

-- \breif: �鿴����ID�Ƿ���������ɵ�������
--  param2:
function IsInGodCaptureGuild(MapID, GuildID)

	if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild == 0 and ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild == 0 then
		return 0
	end

	if GuildID == ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild or GuildID == ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].CaptureGuild then
		return 1
	end

	return 0

end

-- \breif: ����������ڵİ����ǹ��������ط�
function GetRoleGuildDefendOrCapture(MapID, GuildID)
	local IsInGodCapture = IsInGodCaptureGuild(MapID, GuildID)
	if IsInGodCapture ==  1 then

		if GetDefendGuildID(MapID) == GuildID then
			return GodEnum.Defend
		elseif GetCaptureGuildID(MapID) == GuildID then
			return GodEnum.Capture
		end

	else
		return GodEnum.Null
	end
end

-- \breif: �鿴ĳ����ռ�м�������
function GuildCapturePillar(MapID, GuildID)
	if GuildID == 0 then
		return 0
	end

	local Pillar = 0
		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc1 == GuildID then
			Pillar = Pillar + 1
		end

		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc2 == GuildID then
			Pillar = Pillar + 1
		end

		if ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].Npc3 == GuildID then
			Pillar = Pillar + 1
		end

	return Pillar
end

-- \breif:  �õ�����ռ���񼣵ĸ���
--  param1: ����ID
function GetGuildCaptureNum(GuildID)
	if GuildID ==  0 then
		return 0
	end

	local CapNum = 0
	for k, v in pairs(ClanGodMiracle_CurInfo) do
		if ClanGodMiracle_CurInfo[k].DefendGuild == GuildID then
			CapNum  = CapNum  + 1
		end
	end
	return CapNum
end

-- \breif:  �����񼣸����õ���ҵ�ǰʹ�õ�BUFFID
--  param1: ��ȡ�񼣵ĸ���
function GetBuffIDbyGodMiracleNum(godMiracleNum)
	if godMiracleNum == 1 then
		return 2024101
	elseif godMiracleNum == 2  then
		return 2024102
	elseif godMiracleNum == 3 then
		return 2024103
	elseif godMiracleNum == 4 then
		return 2024104
	elseif godMiracleNum == 5 then
		return 2024105
	end
end

-- \breif: ��������
function CreateGodmiracle()
	for k, v in pairs(ClanGodMiracle_CurInfo) do
		if GetOpenFlag(ClanGodMiracle_CurInfo[k].MapID) == 1 then
			local MapID = ClanGodMiracle_CurInfo[k].MapID
			ClanGodMiracle_Proto[MapID].NPCIDObj1 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID1, ClanGodMiracle_Pos[MapID].x1, ClanGodMiracle_Pos[MapID].y1, ClanGodMiracle_Pos[MapID].z1)
			ClanGodMiracle_Proto[MapID].NPCIDObj2 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID2, ClanGodMiracle_Pos[MapID].x2, ClanGodMiracle_Pos[MapID].y2, ClanGodMiracle_Pos[MapID].z2)
			ClanGodMiracle_Proto[MapID].NPCIDObj3 = map.MapCreateCreature(MapID, 0, ClanGodMiracle_Proto[MapID].NPCID3, ClanGodMiracle_Pos[MapID].x3, ClanGodMiracle_Pos[MapID].y3, ClanGodMiracle_Pos[MapID].z3)
		end
	end
end

-- \breif: �õ��񼣵�ռ�����
function InitAllGodMiracle()

	--����
	CreateGodmiracle()

	SetDefendGuildID(MapID_FX ,act.GetActScriptData(ActiveID, 1, 0))
	SetDefendGuildID(MapID_ZL ,act.GetActScriptData(ActiveID, 1, 2))
	SetDefendGuildID(MapID_WS ,act.GetActScriptData(ActiveID, 1, 4))
	SetDefendGuildID(MapID_WY ,act.GetActScriptData(ActiveID, 1, 6))
	SetDefendGuildID(MapID_KL ,act.GetActScriptData(ActiveID, 1, 8))

	for k, v in pairs(ClanGodMiracle_CurInfo) do

		if GetOpenFlag(ClanGodMiracle_CurInfo[k].MapID) == 1 then
			-- �����񼣵ĵ�ǰ״̬,�������ϸ���ɫBUFF
			local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
			if godCurState  == 0 then -- δ������
				ChangePPillarColor(ClanGodMiracle_CurInfo[k].MapID, 9998601)
			elseif godCurState   == 1 then -- ����
				ChangePPillarColor(ClanGodMiracle_CurInfo[k].MapID, 9998901)
			end
		end
	end
end

-- \breif: �ı�������ɫ
--  param2:���ӵ���ɫBUFF
function ChangePPillarColor(MapID, ColorType)
	unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, ColorType, ClanGodMiracle_Proto[MapID].NPCIDObj1)
	unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, ColorType, ClanGodMiracle_Proto[MapID].NPCIDObj2)
	unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, ColorType, ClanGodMiracle_Proto[MapID].NPCIDObj3)
end

Minrecord = 0

-- \breif: �㲥���
function BroadcastGodMsgMin()

	-- �ܶ������ģ���������
	local day = os.date("%A")
	if day ~= "Tuesday" and day ~= "Thursday" and day ~= "Saturday" then
		return
	end

	--���ʱ��
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	--local Sec = tonumber(os.date("%S"))

	Minrecord = Minrecord + 1

	if Minrecord == 10 then
		for k, v in pairs(ClanGodMiracle_CurInfo) do
			-------------------------------------------------------------------------------- world(big)
			-- ��ǰ�������Դ���δ��ռ��״̬����������������ᣡ
			--local godCurState = guild.GetGodMiracleCurState(k-1)
			if ClanGodMiracle_CurInfo[k].DefendGuild == 0  then
				--if Hour < 18 or {Hour > 20 and Min > 30} then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 1400)
						msg.AddMsgEvent(MsgID, 5  , ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchWorldMsgEvent(MsgID)

						Minrecord = 0
				--end
			end
		end

	end



end

broadCastGlag = {flag1 = 0, flag2 = 0, flag3=0, flag4=0}
function ClearBroaCastGlag()
	broadCastGlag.flag1=0
	broadCastGlag.flag2=0
	broadCastGlag.flag3=0
	broadCastGlag.flag4=0
end

BMBroad = 0

-- \breif: �㲥���
-- remark: ÿtick����
function BroadcastGodMsgTick()

	-- �ܶ������ģ���������
	local day = os.date("%A")
	if day ~= "Tuesday" and day ~= "Thursday" and day ~= "Saturday" then
		return
	end

	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec = tonumber(os.date("%S"))

	-------------------------------------------------------------------------------- world(bif)
	-- ���յ�����������ս���۱����Ѿ���ʼ���������ɰ����ڰ��ɽ����н��б�����
	if (Hour == 16 and  Min == 30 and Sec == 0) or (Hour == 17 and  Min == 30 and Sec == 0) or (Hour == 18 and  Min == 30  and Sec == 0) then
		if BMBroad == 0 then
			local MsgIDBM = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgIDBM, 101, 1401)
			msg.DispatchWorldMsgEvent(MsgIDBM)
			BMBroad =  1
		end
	end

	if (Hour == 16 and  Min == 31 ) or (Hour == 17 and  Min == 31 ) or (Hour == 18 and  Min == 31 ) or (Hour == 19 and  Min == 31 ) then
		BMBroad = 0
		ClearBroaCastGlag()
	end

	for k, v in pairs(ClanGodMiracle_CurInfo) do

		-------------------------------------------------------------------------------- guild
		-- ������ȷ���μӽ���20��00�ġ���������ս����������׼����ӭ��ս����
		if Hour == 19 and Min == 35  and Sec == 0 then
			-- ***************�õ��������ID***********************
			SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, guild.GetApplyGuildID(k-1))

				if broadCastGlag.flag1 == 0 then

					--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

						-- �ط�
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 1402)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

						-- ����
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 1402)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))

						if ClanGodMiracle_CurInfo[k].MapID == MapID_KL then
							broadCastGlag.flag1 = 1
						end
				--	end
			end
		end

		-------------------------------------------------------------------------------- guild
		-- ����������ս����10���Ӻ�ʼ������ǰ��������׼��ս����
		if Hour == 19 and Min == 50 and  Sec == 0 then

			if broadCastGlag.flag2 == 0 then
				--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then
					-- �ط�
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 1405)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

					-- ����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 1405)
					msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
					msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))

					if ClanGodMiracle_CurInfo[k].MapID == MapID_KL then
						broadCastGlag.flag2 = 1
					end
				--end
			end
		end

		-- ���ð��ɽ���ս��״̬�������񼣽�������״̬
		if  Hour == 20 and Min == 0 and Sec == 0 then
			--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

				-- ���ϲ�������
				SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, guild.GetApplyGuildID(k-1))
				guild.SetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
				guild.SetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)

				-- ������Ϊ����״̬
				guild.SetGodCurState(k-1, 2)


				--��������ɫ�仯����ɫ
				unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998901)
				unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998901)
				unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998901)

				unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998601, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
				unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998601, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
				unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998601, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

				CalFlag = 0

			--end
		end
	end

		-------------------------------------------------------------------------------- world(big)
		if broadCastGlag.flag3 == 0 then
			-- ���յ�������ս�Ѿ���ʼ��
			if Hour == 20 and Min == 0  and Sec == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1411)
				msg.DispatchWorldMsgEvent(MsgID)
				broadCastGlag.flag3 = 1;
			end
		end


		-------------------------------------------------------------------------------- world
		-- ����������ս�����䶨����ʤ���ɳ�Ա��������Ӿ��飬��Ǯ�͵��ߵ��ʵ�����״̬������ʤ����
		if broadCastGlag.flag4 == 0 then
			if Hour == 20 and Min == 30 and Sec == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1413)
				msg.DispatchWorldMsgEvent(MsgID)

				broadCastGlag.flag4 = 1
			end
		end

end


-- �㲥ս�����
function BroadcastCaptureResult()


	for k, v in pairs(Capture_Result) do

		-- �޽���
		if Capture_Result[k].WinOrLoseFlag  == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1434)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.DispatchWorldMsgEvent(MsgID)
		--�ط�ʤ
		elseif Capture_Result[k].WinOrLoseFlag  == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1432)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].CaptureGuild)
			msg.DispatchWorldMsgEvent(MsgID)

		--��Ӯ
		elseif Capture_Result[k].WinOrLoseFlag  == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 1433)
			msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].DefendGuild)
			msg.AddMsgEvent(MsgID, 19, Capture_Result[k].CaptureGuild)
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end


	--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 1436)
	msg.DispatchWorldMsgEvent(MsgID)--]]

	--guild.SendAllCoptureGodGuildMember()

	ClearGodMiracleResult();
end

-- \breif:  ��ȡ�񼣵�ռ�����ID
--  param1: �񼣵�ID
--  remark: �ɹ�������ID ʧ��:0
function GetGuildByGodMiracle(MapID)
	return ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].DefendGuild
end


