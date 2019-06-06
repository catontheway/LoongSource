-------------------------------------------------------------------------------
-- Copyright (c) 2004 TENGWU Entertainment All rights reserved.
-- filename: GodMiracleCpatureDefine.h
-- author: chdeng
-- actor:
-- data: 2010-01-25
-- last:
-- brief: ������ṹ����
-------------------------------------------------------------------------------
-- \breif: ���е�ͼ��ʼ��ʱ,�����񼣹���״̬����ˢ��ָ�������
function GodMiracleCpature_OnActiveStart(ActiveID)
	if OpenGodMiracleFlag == 0 then
		return
	end

	InitAllGodMiracle()
end


ResultbroadcastFlag = 0

CalFlag = 0

-- �Ƿ��Ѿ���������־
FinishedFlag = 0

-- \breif : ��ͼ��ʱ���º���
--  param1: �ID
--  param2������ʱ��
--  ramark: ����ʱ����ʱ�����񼣵�״̬
function GodMiracleCpature_OnActiveTimer(ActiveID, Sec)

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- �ܶ������ģ���������
	local day = os.date("%A")
	if day ~= "Tuesday" and day ~= "Thursday" and day ~= "Saturday" then
		return
	end


	-- �ڲ���ʱ���²���
	BroadcastGodMsgTick()

	-- �����ʱ���²���
	guild.UpdateGodMiracle()

	---------------------------------------------------------------------------------------------------- ʱ�䵽����ս��
	--���ʱ��������ս��
	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))


	if Hour == 20 and (Min == 30 or Min == 31) and FinishedFlag == 0 then
		CalFlag = 1
	end

	if  FinishedFlag == 1 and Hour == 20 and Min > 31 then
		FinishedFlag = 0
	end

	-- ��ʱ�䵽30��,��¼һ�λ���
	for k,v in pairs(ClanGodMiracle_CurInfo) do
		---------------------------------------------------------------------------------------------------- ���ɽ�ɢ
		-- ��ս״̬�������Ƿ��ɢ
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
		if godCurState ==  1 then

			local GuildIDDiss = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
			if GuildIDDiss ~= 0 then

				-- �鿴ռ���߰����Ƿ��ɢ
				if guild.IsHaveGuild(GuildIDDiss) == false then

					local CaptureGodNum = 0
					-- �鿴������ռ���˶��ٸ���
					for k1, v in pairs(ClanGodMiracle_CurInfo) do
						if GetDefendGuildID(ClanGodMiracle_CurInfo[k1].MapID) == GuildIDDiss then
							CaptureGodNum = CaptureGodNum + 1

							-- �������㲥,�����������
							-- ###�������ƣ��������ѹ����ɱ�������ɽ�������
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1409)
							msg.AddMsgEvent(MsgID, 5 , ClanGodMiracle_CurInfo[k1].GdMiracleID)
							msg.DispatchWorldMsgEvent(MsgID)

							-- ���ɽ�ɢ�����ӱ����ɫ
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998701)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998701)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998701)

							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998801)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998801)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998801)

							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998901)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998901)
							unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998901)

							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998601, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998601, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
							unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998601, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

							-- ���C++��Lua��������
							guild.InitGodMiracle(k1-1)
							ClearGodMiracle(k1)

							-- ������Ϊδռ��״̬
							guild.SetGodCurState(k1-1, 0)


						end
					end

					-- ���ɽ�ɢ,�Ƴ�����������ҵ�����BUFF
					-- �õ����ɵ����г�Ա
					--[[local tmpRoleIDTab = guild.GetAllGuildMember(GuildIDDiss)
					if tmpRoleIDTab ~= nil then
						for k, v in pairs(tmpRoleIDTab) do
							if (RoleTable[k] ~= nil) then
								unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k], GetBuffIDbyGodMiracleNum(CaptureGodNum))
							end
						end
					end--]]

					guild.AddBuffToAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(CaptureGodNum))
				end
			end
		end

		---------------------------------------------------------------------------------------------------- ���ս���Ƿ����
		-- ������״̬��������
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(ClanGodMiracle_CurInfo[k].MapID))
		if godCurState == 2  then

			--BUG
			--if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

				------------------------------------------------------------------------------------------------- 30����˫������
				if CalFlag == 0 then
					ModTick(GetCurInfoMapID(k), 1)
					if  Gettick(GetCurInfoMapID(k)) == GodEnum.StdScoresTick then
						-- �ط�
						local pillarNum = GuildCapturePillar(GetCurInfoMapID(k), GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
						if pillarNum > 0 then
							ModScores(GetCurInfoMapID(k), GodEnum.Defend, pillarNum)
							guild.SetGodGuildPoint(k-1, GodEnum.Defend, pillarNum)
						end

						-- ����
						local pillarNum1 = GuildCapturePillar(GetCurInfoMapID(k), GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))
						if pillarNum1 > 0 then
							ModScores(GetCurInfoMapID(k), GodEnum.Capture, pillarNum1)
							guild.SetGodGuildPoint(k-1,  GodEnum.Capture, pillarNum1)
						end
						SetTick(ClanGodMiracle_CurInfo[k].MapID, 0)
					end
				end

				if CalFlag == 1  then
					--  ���������⴦��
					if GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) == 0 then

						-- ��¼ս�����
						Capture_Result[k].WinOrLoseFlag = 0
						Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
						Capture_Result[k].CaptureGuild  = 0

						-- ���ô��ڿ���״̬
						guild.SetGodCurState(k-1, 1)
						if GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID) == 0 then
							guild.SetGodCurState(k-1, 0)
						end

						-- ˫�����ɽ��ս��״̬
						guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
						guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)

						aux.WriteLog("begin:Cal(Capture==0 ):CalCaptureContributionAndExploit")
						--��㹱�׺͹�ѫ����
						guild.CalCaptureContributionAndExploit(k-1, GodEnum.Defend)
						aux.WriteLog("end:Cal(Capture==0 ):CalCaptureContributionAndExploit")

						-- ��ס��,������κ�BUFF
						-- �����ɳɹ��ĺ������Լ����񼣣�
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 1430)
						msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
						msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

						--ResetGodMiracle(k)

					elseif GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0 then

						if  GetScores(GetCurInfoMapID(k), GodEnum.Defend) >= GetScores(GetCurInfoMapID(k), GodEnum.Capture) then -- �ط�ʤ

							-- ȥ�������ж�buff
							guild.RemoveBuffFromAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024801)
							guild.RemoveBuffFromAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024901)

							-- ȥ�������ж�buff
							guild.RemoveBuffFromAllGuildMember(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024801)
							guild.RemoveBuffFromAllGuildMember(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024901)

							aux.WriteLog("defend win: Remove Friendly or Enemy buff\r\n")

							-- ��¼ս�����
							Capture_Result[k].WinOrLoseFlag = 1
							Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
							Capture_Result[k].CaptureGuild  = GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID)

							-- ���ô��ڿ���״̬
							guild.SetGodCurState(k-1, 1)
							aux.WriteLog("defend win: SetGodCurState(k-1, 1)\r\n")

							-- ˫�����ɽ��ս��״̬
							guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							aux.WriteLog("defend win: UnsetGuildStatus\r\n")

							--��㹱�׺͹�ѫ����
							aux.WriteLog("begin:Cal(Defend Win):CalCaptureContributionAndExploit\r\n")
							guild.CalCaptureContributionAndExploit(k-1, GodEnum.Defend)
							aux.WriteLog("end:Cal(Defend Win):CalCaptureContributionAndExploit\r\n")

							-- ��ס��,������κ�BUFF
							-- �����ɳɹ��ĺ������Լ����񼣣�
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 1)
							msg.AddMsgEvent(MsgID, 1, 1430)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

							--ResetGodMiracle(k)

						elseif GetScores(GetCurInfoMapID(k), GodEnum.Defend) < GetScores(GetCurInfoMapID(k), GodEnum.Capture)  then -- ����ʤ

							-- ȥ�������ж�buff
							guild.RemoveBuffFromAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024801)
							guild.RemoveBuffFromAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024901)

							-- ȥ�������ж�buff
							guild.RemoveBuffFromAllGuildMember(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024801)
							guild.RemoveBuffFromAllGuildMember(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 2024901)

							aux.WriteLog("capture win: Remove Friendly or Enemy buff\r\n")

							-- ��¼ս�����
							Capture_Result[k].WinOrLoseFlag = 2
							Capture_Result[k].DefendGuild   = GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID)
							Capture_Result[k].CaptureGuild  = GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID)

							-- ���ô��ڿ���״̬
							guild.SetGodCurState(k-1, 1)
							aux.WriteLog("capture win: SetGodCurState(k-1, 1)\r\n")

							-- ˫�����ɽ��ս��״̬
							guild.UnsetGuildStatus(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							guild.UnsetGuildStatus(GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID), 8)
							aux.WriteLog("capture win: UnsetGuildStatus\r\n")

							--��㹱�׺͹�ѫ����
							aux.WriteLog("begin:Cal(Capture Win):CalCaptureContributionAndExploit\r\n")
							guild.CalCaptureContributionAndExploit(k-1, GodEnum.Capture)
							aux.WriteLog("end:Cal(Capture Win):CalCaptureContributionAndExploit\r\n")

							---------------------------------------------------------------------------------------------------- BUFF��������

							-- ��ʧ�񼣣����ӽ�һ��
							local DefendGodNum = 0
							DefendGodNum = GetGuildCaptureNum(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
							if DefendGodNum == 1 then
								--[[local tmpRoleIDTab = guild.GetAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
								if tmpRoleIDTab ~= nil then
									for k5, v in pairs(tmpRoleIDTab) do
										if tmpRoleIDTab[k5] ~= nil then
											unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k5], GetBuffIDbyGodMiracleNum(1))
										end
									end
								end--]]
								guild.RemoveBuffFromAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(1))
								aux.WriteLog("defend lose: 1 Remove buff\r\n")
							elseif DefendGodNum > 1 then

								-- ���Ƴ��߽�BUF
								--[[local tmpRoleIDTab = guild.GetAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
								if tmpRoleIDTab ~= nil then
									for k4, v in pairs(tmpRoleIDTab) do
										if tmpRoleIDTab[k4] ~= nil then
											unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k4], GetBuffIDbyGodMiracleNum(DefendGodNum))
											unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k4], GetBuffIDbyGodMiracleNum(DefendGodNum-1), tmpRoleIDTab[k4])
										end
									end
								end--]]

								guild.RemoveBuffFromAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(DefendGodNum))
								-- ���ӵͼ�BUF
								guild.AddBuffToAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(DefendGodNum - 1))
							end
							aux.WriteLog("defend lose: > 1 Remove buff\r\n")

							-- ��ʼ����
							SetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID, GetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID))
							SetCaptureGuildID(ClanGodMiracle_CurInfo[k].MapID, 0)

							-- �����񼣷�,����һ����
							local CaptureGodNum1 = 0
							CaptureGodNum1 = GetGuildCaptureNum(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
							if CaptureGodNum1  > 1 then
								-- ���Ƴ��ͼ�BUFF
								--[[local tmpRoleIDTab = guild.GetAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
								if tmpRoleIDTab ~= nil then
									for k3, v in pairs(tmpRoleIDTab) do
										if (tmpRoleIDTab[k3] ~= nil) then
											unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k3], GetBuffIDbyGodMiracleNum(CaptureGodNum1-1))
											unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k3], GetBuffIDbyGodMiracleNum(CaptureGodNum1), tmpRoleIDTab[k3])
										end
									end
								end--]]

								guild.RemoveBuffFromAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(CaptureGodNum1-1))
								-- ���Ӹ߽�BUFF
								guild.AddBuffToAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(CaptureGodNum1))

								aux.WriteLog("capture win: > 1 Add buff\r\n")
							elseif CaptureGodNum1  == 1 then
								-- ���Ƴ��߽�BUF
								--[[local tmpRoleIDTab = guild.GetAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))
								if tmpRoleIDTab ~= nil then
									for k2, v in pairs(tmpRoleIDTab) do
										if (tmpRoleIDTab[k2] ~= nil) then
											unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, tmpRoleIDTab[k2], GetBuffIDbyGodMiracleNum(1), tmpRoleIDTab[k2])
										end
									end
								end--]]
								guild.AddBuffToAllGuildMember(GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID), GetBuffIDbyGodMiracleNum(1))
							end
							aux.WriteLog("capture win: 1 Add buff\r\n")


							-- ��ȡ�����ӹ㲥
							local MsgID1 = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID1, 13, 1)

							if CaptureGodNum == 1 then
								msg.AddMsgEvent(MsgID1, 1, 1421)
							elseif CaptureGodNum == 2 then
								msg.AddMsgEvent(MsgID1, 1, 1422)
							elseif CaptureGodNum == 3 then
								msg.AddMsgEvent(MsgID1, 1, 1423)
							elseif CaptureGodNum == 4 then
								msg.AddMsgEvent(MsgID1, 1, 1424)
							elseif CaptureGodNum == 5 then
								msg.AddMsgEvent(MsgID1, 1, 1435)
							end
							msg.AddMsgEvent(MsgID1, 5  , ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.AddMsgEvent(MsgID1, 19,  guildID)
							msg.DispatchWorldMsgEvent(MsgID1)

							---------------------------------------------------------------------------------------------------- BUFF��������

							-- ����ͨ�������������ˡ����񼣵�ռ��Ȩ�����г�Ա�������״̬��ͬʱ��ҿ�ǰ���񼣹���������Ʒ
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 101, 1415)
							msg.AddMsgEvent(MsgID, 5, ClanGodMiracle_CurInfo[k].GdMiracleID)
							msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID))

							--ResetGodMiracle(k)

						end
					end

					-- �����ķ�ʤ,����������Ϊ��ɫ
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998701)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998701)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998701)

					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998801)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998801)
					unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998801)

					if GetDefendGuildID(ClanGodMiracle_CurInfo[k].MapID) ~= 0  then

						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998601)
						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998601)
						unit.CancelBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998601)

						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1, 9998901, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj1)
						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2, 9998901, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj2)
						unit.AddBuff(ClanGodMiracle_CurInfo[k].MapID, 0, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3, 9998901, ClanGodMiracle_Proto[ClanGodMiracle_CurInfo[k].MapID].NPCIDObj3)

					end

					aux.WriteLog("Setting Pillar corlor\r\n")

					ResetGodMiracle(k)

					ClearBroaCastGlag()
					ResultbroadcastFlag = 1

				end
			--end
		end
	end

	if ResultbroadcastFlag == 1 then
		aux.WriteLog("Broadcast cal result!\r\n")
		CalFlag = 0
		FinishedFlag	= 1
		BroadcastCaptureResult()
		ResultbroadcastFlag  = 0
		EnterFlag = 0

	end
end

-- \breif : �ÿ����ˢ��
--  param1: �ID
function GodMiracleCpature_OnActiveTimeMin(ActiveID)

	if OpenGodMiracleFlag == 0 then
		return
	end
	--  ����������,��¼��Ծʱ��
	guild.UpdateGodMiracleMin()

	BroadcastGodMsgMin()
end


-- \breif : ��ұ�ɱ
--  param1:
function GodMiracleCpature_OnRoleDie(MapID, InstanceID, BeKilledID, KillerID)
    --���鱦�������ؽű�
	if unit.IsPlayer(BeKilledID) and not unit.IsPlayer(KillerID) then
	    local TypeID = cre.GetCreatureTypeID(MapID,InstanceID, KillerID)
		if TypeID ==  1535307 then
		    cre.MonsterSay(MapID, InstanceID, KillerID, 50081)
		    unit.AddBuff(MapID, InstanceID, KillerID, 9431401, KillerID)
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3168)    --�������ɱ����XXX����������ø����ײ��ˣ�
			msg.AddMsgEvent(MsgID, 2, BeKilledID)
			msg.DispatchMapMsgEvent(MsgID, 3017299407, -1)
		end
	end

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- �ܶ������ģ���������
	local day = os.date("%A")
	if day ~= "Tuesday" and day ~= "Thursday" and day ~= "Saturday" then
		return
	end


	-- �Ƿ�����������
	local IsInArea = map.IsInArea(MapID, InstanceID, KillerID, GetAreaID(MapID))
	if IsInArea == 0 then
		return
	end

	-- ������״̬��������
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2  and GetCaptureGuildID(MapID) ~= 0 then
		return
	end

	-- �õ�ɱ���߰���ID
	local killerGuildID = guild.GetRoleGuildID(KillerID)
	if killerGuildID ~= 4294967295 then
		local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, killerGuildID)
			if DOrCFlag ~= GodEnum.Null then
				guild.SetGuildRoleDie(GetGodMiracleIDMinOne(MapID), KillerID, BeKilledID, DOrCFlag)
			end
	end

end



local EnterFlag = 0

-- \breif : ��ҽ�����
--  param3: ����ű���������ID
--  param4: ����Ľű�����ID
function GodMiracleCpature_OnRoleEnterArea(MapID, InstanceID, OwnerID, ObjAreaID)

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- �ܶ������ģ���������
	local day = os.date("%A")
	if day ~= "Tuesday" and day ~= "Thursday" and day ~= "Saturday" then
		return
	end


	-- �Ƿ�����������
	local IsInArea = map.IsInArea(MapID, InstanceID, OwnerID, GetAreaID(MapID))
	if IsInArea == false then
		return
	end

	-- ������״̬��������
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2 or GetCaptureGuildID(MapID) == 0 then
		return
	end

	-- ������ҽ����������ϲ㴴�����񼣽����������־
	if GetAreaID(MapID) == ObjAreaID then
		-- ��C++�㴴����Ҳ�������ҿ��Լ�¼��Ծʱ��
		local guildID = guild.GetRoleGuildID(OwnerID)
		if guildID ~= 4294967295 then
			local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
			if DOrCFlag ~= GodEnum.Null then

				-- ���ý�ɫ�����񼣱�־
				guild.GuildRoleEnterGodArea(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag)

				-- ���û�Ծʱ��
				guild.SetRoleStartActiveTimes(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag, 1)



				if GetDefendGuildID(MapID) == guildID then
					unit.CancelBuff(MapID, 0, OwnerID, 2024801)
					unit.AddBuff(MapID, 0, OwnerID, 2024901, OwnerID)
				elseif GetCaptureGuildID(MapID) == guildID then
					unit.CancelBuff(MapID, 0, OwnerID, 2024901)
					unit.AddBuff(MapID, 0, OwnerID, 2024801, OwnerID)
				end

				if EnterFlag == 0 then

				--  ����������й㲥
				-- ���İ������ڲ����������ս�������ж��ѱ�Ϊ����սģʽ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1416)
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)

				-- �ر���������
				role.UnSetRoleState(OwnerID, 1, 1)

				EnterFlag =  1

				end
			end
		end
	end

end

-- \breif : ����뿪������
--  param3: �뿪�ű���������ID
--  param4: �뿪�Ľű�����ID
function GodMiracleCpature_OnRoleLeaveArea(MapID, InstanceID, OwnerID, ObjAreaID)

	if OpenGodMiracleFlag == 0 then
		return
	end

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- �ܶ������ģ���������
	local day = os.date("%A")
	if day ~= "Tuesday" and day ~= "Thursday" and day ~= "Saturday" then
		return
	end


	-- ������״̬��������
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState ~= 2  or  GetCaptureGuildID(MapID) == 0 then
		return
	end

	-- ��������뿪������
	if GetAreaID(MapID) == ObjAreaID then
		local guildID = guild.GetRoleGuildID(OwnerID)
		if guildID ~= 4294967295 then

			local DOrCFlag = GetRoleGuildDefendOrCapture(MapID, guildID)
			if DOrCFlag ~= GodEnum.Null then

				guild.SetRoleStartActiveTimes(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag, 0)

				-- ȡ�������ɫ����BUFF
				--if GetDefendGuildID(MapID) == guildID then
					unit.CancelBuff(MapID, 0, OwnerID, 2024901)
				--elseif GetCaptureGuildID(MapID) == guildID then
					unit.CancelBuff(MapID, 0, OwnerID, 2024801)
				--end

				--�����뿪�����������ս�ѽ����������ж��ѱ�Ϊͨ��ģʽ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1417)
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)--]]

				--֪ͨ�ͻ���
				guild.RoleLeaveArea(GetGodMiracleIDMinOne(MapID), OwnerID, GetRoleGuildDefendOrCapture(MapID, guildID))

				EnterFlag = 0
			end
		end
	else
		local guildID = guild.GetRoleGuildID(OwnerID)
		if guildID ~= 4294967295 then
			if guildID == GetDefendGuildID(MapID) or guildID == GetCaptureGuildID(MapID) then

				guild.SetRoleStartActiveTimes(GetGodMiracleIDMinOne(MapID), OwnerID, DOrCFlag, 0)

				-- ȡ�������ɫ����BUFF
				--if GetDefendGuildID(MapID) == guildID then
					unit.CancelBuff(MapID, 0, OwnerID, 2024901)
				--elseif GetCaptureGuildID(MapID) == guildID then
					unit.CancelBuff(MapID, 0, OwnerID, 2024801)
				--end

				--�����뿪�����������ս�ѽ����������ж��ѱ�Ϊͨ��ģʽ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1417)
				msg.DispatchRoleMsgEvent(OwnerID, MsgID)--]]

				--֪ͨ�ͻ���
				guild.RoleLeaveArea(GetGodMiracleIDMinOne(MapID), OwnerID, GetRoleGuildDefendOrCapture(MapID, guildID))

				EnterFlag = 0
			end
		end
	end
end

-- \breif : ���ָ�������Ƿ��ܱ��ͷ�
--  param3: SkillID(����ID)
--  param4: �ͷż�����ID
--  param5: Ŀ��ID
--  return: �����ʾ��Ϣ�Խű���Ϣ�����ͻ���,������ֻҪ������0��Ϊ����,c++����������
function GodMiracleCpature_CanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- �ܶ������ģ���������
	local day = os.date("%A")
	if day ~= "Tuesday" and day ~= "Thursday" and day ~= "Saturday" then
		return
	end


	-- ���ڿ���״̬,��������
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState  == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,26, 1427)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 999
	end


	-- ��ȡ����ID(eroor:�ӿں���Ӧ����û�а��ɵ�����·���0��Ӧ�÷������޴���ֵ)
	local GuildID
	GuildID = guild.GetRoleGuildID(OwnerID);
	-- ��ȡ��Ұ���ְλ(GuildPos==nil ����ɲ�����)
	local GuildPos = -1
	GuildPos = guild.GetRoleGuildPos(GuildID , OwnerID);
	if GuildPos == nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,26, 1419)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 999
	end

	-- ���û�п�������״̬,���ɿ���
		-- ����δ��������״̬���޷����п����񼣲�����
	if role.IsRoleInStatus(OwnerID, 4) == false and role.IsRoleInStatus(OwnerID, 1048576) == false then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1403)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 999
	end

	if godCurState  == 2 and GetDefendGuildID(MapID) ~= 0 then
		-- ��Ҳ���������ɷ�Χ!
		if GetDefendGuildID(MapID) ~= GuildID and GetCaptureGuildID(MapID) ~= GuildID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1429)
			msg.DispatchRoleMsgEvent(OwnerID, MsgID)

			return 999
		end
	end

	-- δ��ս���������ᣬ���ÿ���
	if godCurState  == 2 and GetDefendGuildID(MapID) ~= 0 and GetCaptureGuildID(MapID) == 0 then
		return 999
	end

	-- ���������ڱ����ɣ����ÿ���
	local TargetTypeID = unit.GetUnitTypeID(MapID, InstanceID, TargetID)
	local NpcIndex = GetGodStackeIndex(MapID, TargetTypeID)
	if NpcIndex == GodEnum.Null then return 999 end
	local guildID1 = GetCapGodStacke(MapID, NpcIndex)
	if guildID1 ~= 0 and guildID1 == GuildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1425)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 999
	end

	--�ȼ��������ɿ���
	local OwnerLevel = role.GetRoleLevel(MapID, InstanceID, OwnerID)
	if OwnerLevel < 30 then
		return
	end

	-- ���ӿ���Ч��
	unit.AddBuff(MapID, 0, TargetID, 9998501, TargetID)
end

-- \breif : �����ͷ�����Ч��
--  param3: SkillID(����ID)
--  param4: �ͷż�����ID
--  return: 1:֪ͨ�ϲ㲻Ҫ���ô˼���Ч�� 0:���෴
function GodMiracleCpature_CastSkill(MapID, InstanceID, SkillID, OwnerID, TargetID, TargetTypeID)

	-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return
	end

	-- �õ�ռ�����Ǹ�����
	local GodStackeIndex = GetGodStackeIndex(MapID, TargetTypeID)
	if GodStackeIndex == GodEnum.Null then return 1 end

	-- ��Ұ��ɽ�ɢ,���ɿ���
	local guildID = 0
	guildID = guild.GetRoleGuildID(OwnerID)
	local GuildPos
	GuildPos  = guild.GetRoleGuildPos(guildID, OwnerID);
	if GuildPos == nil then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1404)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end


	-- ���û�п�������״̬,���ɿ���
	if role.IsRoleInStatus(OwnerID, 1) == true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1406)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end
	if role.IsRoleInStatus(OwnerID, 4) == false and role.IsRoleInStatus(OwnerID, 1048576) == false then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1406)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end

	-- ���������ڱ����ɣ����ÿ���
	local guildID1 = GetCapGodStacke(MapID, TargetTypeID)
	if guildID1 ~= 0 and guildID1  == guildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1415)
		msg.DispatchRoleMsgEvent(OwnerID, MsgID)
		return 1
	end

	--�ȼ��������ɿ���
	local OwnerLevel = role.GetRoleLevel(MapID, InstanceID, OwnerID)
	if OwnerLevel < 30 then
		return
	end


	-- ״̬0��1�����������һ��
	local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
	if godCurState  == 0 or (godCurState == 2 and GetDefendGuildID(MapID) == 0 ) then
		-- ���ð��ɻ�ȡ����
		SetCapGodStacke(MapID, GodStackeIndex, guildID)

		-- ȡ������Ч��BUFF
		unit.CancelBuff(MapID, 0, TargetID, 9998501)

		-- �Ƴ���ɫЧ��
		unit.CancelBuff(MapID, 0, TargetID, 9998601)

		-- �����ǹ���Ч��
		unit.AddBuff(MapID, 0, TargetID, 9998701, TargetID)

		-- ������Χ�ڹ㲥
		-- �����������������������ƣ����ɵġ���������������������ƣ��ѿ����ˡ������������������ƣ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1415)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchWorldMsgEvent(MsgID)

		local piller = GuildCapturePillar(MapID, guildID)
		if piller == 2 then
			-- ����c++��ռ�����
			guild.SetCaptureGodGuildID(GetGodMiracleIDMinOne(MapID), guildID)


			if godCurState == 0 then
				-- ���ô��ڿ���״̬
				guild.SetGodCurState(GetGodMiracleIDMinOne(MapID), 1)
			end

			ClearGodMiracle(ClanGodMiracle_Proto[MapID].ClanGodMiracleID)

			--����lua�˰���ռ����
			SetDefendGuildID(MapID, guildID)


			-- ��ȡ��.��������һ��
			local DefendGodNum = 0
			DefendGodNum = GetGuildCaptureNum(guildID)
			if 	DefendGodNum == 1 then
				guild.AddBuffToAllGuildMember(guildID, GetBuffIDbyGodMiracleNum(1))
			elseif DefendGodNum > 1 then
				-- ���Ƴ��߽�BUF
				--[[local tmpRoleIDTab = guild.GetAllGuildMember(guildID)
				if tmpRoleIDTab ~= nil then
					for k, v in pairs(tmpRoleIDTab) do
						if (tmpRoleIDTab[k] ~= nil) then
							unit.CancelBuff(MapID, 0, tmpRoleIDTab[k], GetBuffIDbyGodMiracleNum(DefendGodNum-1))
						end
					end
				end--]]
				guild.RemoveBuffFromAllGuildMember(guildID, GetBuffIDbyGodMiracleNum(DefendGodNum-1))
				guild.AddBuffToAllGuildMember(guildID, GetBuffIDbyGodMiracleNum(DefendGodNum))
			end

			-- ###�������ƣ���####���������ƣ�ռ�죡�����������г�Ա�����####����״̬���ƣ���״̬��������
			local MsgID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID1, 13, 1)

			if DefendGodNum == 1 then
				msg.AddMsgEvent(MsgID1, 1, 1421)
			elseif DefendGodNum == 2 then
				msg.AddMsgEvent(MsgID1, 1, 1422)
			elseif DefendGodNum == 3 then
				msg.AddMsgEvent(MsgID1, 1, 1423)
			elseif DefendGodNum == 4 then
				msg.AddMsgEvent(MsgID1, 1, 1424)
			elseif DefendGodNum == 5 then
				msg.AddMsgEvent(MsgID1, 1, 1435)
			end
			msg.AddMsgEvent(MsgID1, 5  , ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].GdMiracleID)
			msg.AddMsgEvent(MsgID1, 19,  guildID)
			msg.DispatchWorldMsgEvent(MsgID1)

			-- �����ڴ�����㲥
			-- ����ͨ�������������ˡ����񼣵�ռ��Ȩ�����г�Ա�������״̬��ͬʱ��ҿ�ǰ���񼣹���������Ʒ��
			local MsgID2 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID2, 101, 1414)
			msg.AddMsgEvent(MsgID2, 5, ClanGodMiracle_CurInfo[ClanGodMiracle_Proto[MapID].ClanGodMiracleID].GdMiracleID)
			msg.DispatchGuildMsgEvent(MsgID2, guildID)


			-- ��һ��ռ���񼣲��������״̬,��������
			--ChangePPillarColor(MapID, 9998801)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, 9998601)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, 9998601)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, 9998601)

			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, 9998701)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, 9998701)
			unit.CancelBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, 9998701)

			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj1, 9998901, ClanGodMiracle_Proto[MapID].NPCIDObj1)
			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj2, 9998901, ClanGodMiracle_Proto[MapID].NPCIDObj2)
			unit.AddBuff(MapID, 0, ClanGodMiracle_Proto[MapID].NPCIDObj3, 9998901, ClanGodMiracle_Proto[MapID].NPCIDObj3)

			ResetGodMiracle(ClanGodMiracle_Proto[MapID].ClanGodMiracleID)

		end

	elseif godCurState == 2 then

		-- ����ʱ�䵽,���ɿ���
		local Hour = tonumber(os.date("%H"))
		local Min  = tonumber(os.date("%M"))
		if Hour == 20 and Min >= 30  then
			return
		end

		-- ���ð��ɻ�ȡ����
		SetCapGodStacke(MapID, GodStackeIndex, guildID)

		-- ������Χ�ڹ㲥
		-- �����������������������ƣ����ɵġ���������������������ƣ��ѿ����ˡ������������������ƣ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1415)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchGuildMsgEvent(MsgID, GetCaptureGuildID(MapID))

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1415)
		msg.AddMsgEvent(MsgID, 19, guildID)
		msg.AddMsgEvent(MsgID, 2, OwnerID)
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.DispatchGuildMsgEvent(MsgID, GetDefendGuildID(MapID))


		-- ȡ������Ч��BUFF
		unit.CancelBuff(MapID, 0, TargetID, 9998501)

		-- ���ݹ������ط��仯���ӵ���ɫ
		if guildID ==GetDefendGuildID(MapID) then
			unit.CancelBuff(MapID, 0, TargetID, 9998601)
			unit.CancelBuff(MapID, 0, TargetID, 9998901)

			unit.CancelBuff(MapID, 0, TargetID, 9998701)
			unit.AddBuff(MapID, 0, TargetID, 9998801, TargetID)
		elseif guildID == GetCaptureGuildID(MapID) then
			unit.CancelBuff(MapID, 0, TargetID, 9998601)
			unit.CancelBuff(MapID, 0, TargetID, 9998901)

			unit.CancelBuff(MapID, 0, TargetID, 9998801)
			unit.AddBuff(MapID, 0, TargetID, 9998701, TargetID)
		end

	end

	return 0
end

--- breif: �Ծ��д��͹��ܵĵ���������
function GodMiracleCapture_ItemCanUse(MapID, InstanceID, TypeID, TargetID)

	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))

	if Hour == 20 and (Min <= 30 and Min >= 0)then
		-- ����������᷶Χ��
		local guildID = 0
		guildID = guild.GetRoleGuildID(TargetID)
		local GuildPos
		GuildPos  = guild.GetRoleGuildPos(guildID, TargetID);
		if GuildPos ~= nil then
			if GetDefendGuildID(MapID) == guildID or GetCaptureGuildID(MapID) == guildID then
				--�Ƿ���������������
				local IsInArea = map.IsInArea(MapID, InstanceID, TargetID, GetAreaID(MapID))
				if IsInArea == 0 then
					return 0, 0
				else
					return -1, 1
				end
			end
		end
	end
	return 0, 0
end

------------------------------------------------------------------------------
-- register

-- ��ע���ʼ������ʱ���²���
aux.RegisterActEvent(100, 1, "GodMiracleCpature_OnActiveTimer")
aux.RegisterActEvent(100, 2, "GodMiracleCpature_OnActiveStart")
aux.RegisterActEvent(100, 4, "GodMiracleCpature_OnActiveTimeMin")

-- map
aux.RegisterMapEvent("m03", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("m04", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("m05", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("m06", 5, "GodMiracleCpature_OnRoleDie")
aux.RegisterMapEvent("m07", 5, "GodMiracleCpature_OnRoleDie")

aux.RegisterMapEvent("m03", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("m04", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("m05", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("m06", 7, "GodMiracleCpature_OnRoleEnterArea")
aux.RegisterMapEvent("m07", 7, "GodMiracleCpature_OnRoleEnterArea")

aux.RegisterMapEvent("m03", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("m04", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("m05", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("m06", 23, "GodMiracleCpature_OnRoleLeaveArea")
aux.RegisterMapEvent("m07", 23, "GodMiracleCpature_OnRoleLeaveArea")


-- skill
-- ���鼼��ע�����ʹ���¼���
aux.RegisterSkillEvent(9001701, 0, "GodMiracleCpature_CanCast")
aux.RegisterSkillEvent(9001701, 2, "GodMiracleCpature_CastSkill")

-- ʹ���ɵ����|�ɵ����|�ɵ����|����׷��
aux.RegisterItemEvent(1310001, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360003, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360102, 0, "GodMiracleCapture_ItemCanUse")
aux.RegisterItemEvent(1360110, 0, "GodMiracleCapture_ItemCanUse")



