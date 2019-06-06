WorldCup_Creature = {}

WorldCup_On = 0

function WorldCup_OnStart(actID)

	WorldCup_On = 1

	WorldCup_Creature[1] = map.MapCreateCreature(3017298383, -1, 4900473, 861, 10000, 2279) --���򱦱�A
	WorldCup_Creature[2] = map.MapCreateCreature(3017298383, -1, 4900474, 861, 10000, 2271) --���򱦱�B
	WorldCup_Creature[3] = map.MapCreateCreature(3017298383, -1, 4900475, 869, 10000, 2279) --���򱦱�C
	WorldCup_Creature[4] = map.MapCreateCreature(3017298383, -1, 4900476, 869, 10000, 2271) --���򱦱�D
	WorldCup_Creature[5] = map.MapCreateCreature(3017298383, -1, 4900477, 902, 10000, 2279) --���򱦱�E
	WorldCup_Creature[6] = map.MapCreateCreature(3017298383, -1, 4900478, 902, 10000, 2271) --���򱦱�F
	WorldCup_Creature[7] = map.MapCreateCreature(3017298383, -1, 4900479, 910, 10000, 2279) --���򱦱�G
	WorldCup_Creature[8] = map.MapCreateCreature(3017298383, -1, 4900480, 910, 10000, 2271) --���򱦱�H
	WorldCup_Creature[9] = map.MapCreateCreature(3017298383, -1, 4900481, 887, 10000, 2276) --ͶƱ����˵����
	WorldCup_Creature[10] = map.MapCreateCreature(3017298383, -1, 4900483, 872, 10000, 2247) --�ư��ϰ�
	WorldCup_Creature[11] = map.MapCreateCreature(3017298383, -1, 4900484, 904, 10000, 2247) --�ư��ϰ�
	WorldCup_Creature[12] = map.MapCreateCreature(3017298127, -1, 4900481, 2159, 6646, 2248) --ͶƱ����˵����--����

end

function WorldCup_OnEnd(actID)

	for i=1,11 do
		map.MapDeleteCreature(3017298383, -1, WorldCup_Creature[i])
	end

	map.MapDeleteCreature(3017298127, -1, WorldCup_Creature[12])

	WorldCup_On = 0

end


function WorldCup_OnTimerMin(actID)

	local curhour = tonumber(os.date("%H"))

	if curhour == 17 then

		WorldCup_Today_Team = -1
		act.SetActScriptData(145, 1, 35, 0)
		act.SetActScriptData(145, 1, 36, 0)
		act.SetActScriptData(145, 1, 37, 0)
		act.SaveActScriptData(145)

	end

	if curhour%2==1 and curmin==33 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 3269)	--�ָ���Թ���
		msg.DispatchWorldMsgEvent(MsgID)

	end
end

--ע��û������¼�
aux.RegisterActEvent(145, 4, "WorldCup_OnTimerMin")	--�ID
aux.RegisterActEvent(145, 2, "WorldCup_OnStart")	--�ID
aux.RegisterActEvent(145, 3, "WorldCup_OnEnd")		--�ID

--�����򱦱��Ի�
function ZuQiuBaoBei_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local SupportGroupID = TargetTypeID-4900473

	if TalkIndex == -1 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 3201)			--��ͨ�Ի�
		msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
		msg.AddMsgEvent(MsgID, 21,	4)				--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3234)			--�ź�����
		msg.AddMsgEvent(MsgID, 21,	5)				--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+1)	--������
		msg.AddMsgEvent(MsgID, 21,	6)				--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+2)	--������
		msg.AddMsgEvent(MsgID, 21,	7)				--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+3)	--������
		msg.AddMsgEvent(MsgID, 21,	8)				--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+4)	--������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex>=5 and TalkIndex<=8 then

		local VoteDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["VoteDate"])
		local curdate = tonumber(os.date("%j"))

		local curday = tonumber(os.date("%d"))
		local curmonth = tonumber(os.date("%m"))

		if curmonth==7 and curday>1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3235)			--�Ѿ����������׶��޷�ͶƱ
			msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif curdate~=VoteDate then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3236)			--��û���ź�����
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.AddMsgEvent(MsgID, 21, 4)				--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 3234)			--�ź�����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		else

			local i = role.GetRoleItemNum(RoleID, 3304101)

			if i<1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3237)			--û�����籭ѡƱ
				msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else

				local CurTeam = SupportGroupID*4 + TalkIndex-4
				local SupportTeam = role.GetRoleScriptData(RoleID, 1, RoleDataType["SupportTeam"])

				local VoteNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["VoteNum"])

				if CurTeam~=SupportTeam and SupportTeam~=0 then

					local FinishiDate=26

					if curmonth ~=6 or curday>FinishiDate then

						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 3238)			--С�����Ѿ�ȫ�����������ɸ���֧�ֶ���
						msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
						msg.DispatchRoleMsgEvent(RoleID, MsgID)

					else

						role.RemoveFromRole(MapID, InstanceID, RoleID, 3304101, 1, 420)
						role.SetRoleScriptData(RoleID, 1, RoleDataType["SupportTeam"], CurTeam)

						local TempVote1 = act.GetActScriptData(145,1,SupportTeam)
						local TempVote2 = act.GetActScriptData(145,1,CurTeam)

						act.SetActScriptData(145,1,SupportTeam, TempVote1-VoteNum)
						act.SetActScriptData(145,1,CurTeam, TempVote2+VoteNum)

						act.SaveActScriptData(145)

						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 3239)			--����֧�ֶ���ɹ�
						msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
						msg.AddMsgEvent(MsgID, 21,	5)				--ȷ����ť
						msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+1)	--������
						msg.AddMsgEvent(MsgID, 21,	6)				--ȷ����ť
						msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+2)	--������
						msg.AddMsgEvent(MsgID, 21,	7)				--ȷ����ť
						msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+3)	--������
						msg.AddMsgEvent(MsgID, 21,	8)				--ȷ����ť
						msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+4)	--������
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end

				else
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3304101, 1, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["SupportTeam"], CurTeam)

					local TempVote = act.GetActScriptData(145,1,CurTeam)
					local VoteSum = act.GetActScriptData(145,1,33)

					act.SetActScriptData(145,1,CurTeam, TempVote+1)
					act.SetActScriptData(145,1,33, VoteSum+1)
					act.SaveActScriptData(145)

					role.SetRoleScriptData(RoleID, 1, RoleDataType["VoteNum"], VoteNum+1)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3240)			--ͶƱ�ɹ�
					msg.AddMsgEvent(MsgID, 9, VoteNum+1)
					msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
					msg.AddMsgEvent(MsgID, 21,	5)				--ȷ����ť
					msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+1)	--������
					msg.AddMsgEvent(MsgID, 21,	6)				--ȷ����ť
					msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+2)	--������
					msg.AddMsgEvent(MsgID, 21,	7)				--ȷ����ť
					msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+3)	--������
					msg.AddMsgEvent(MsgID, 21,	8)				--ȷ����ť
					msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+4)	--������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end

	elseif TalkIndex==4 then

		local VoteDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["VoteDate"])
		local curdate = tonumber(os.date("%j"))

		if VoteDate==curdate then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3241)			--һ��ֻ�ܽ���һ���ź�����
			msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
			msg.AddMsgEvent(MsgID, 21,	5)				--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+1)	--������
			msg.AddMsgEvent(MsgID, 21,	6)				--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+2)	--������
			msg.AddMsgEvent(MsgID, 21,	7)				--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+3)	--������
			msg.AddMsgEvent(MsgID, 21,	8)				--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+4)	--������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		else

			local j = role.GetRoleItemNum(RoleID, 3304100)

			if j<1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3251)			--��û�������ŵ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else

				local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				local LevelUpNeedExp = role.GetRoleLevelUpExp(MapID, InstanceID, RoleID)
				local addexp = LevelUpNeedExp * 100 / (rolelevel+10)^2
				role.AddRoleExp(MapID, InstanceID, RoleID, addexp)

				role.RemoveFromRole(MapID, InstanceID, RoleID, 3304100, 1, 420)

				local addsilver = 0

				if rolelevel<15 then
					addsilver = 0
				elseif rolelevel<=30 then
					addsilver = 30000
				elseif rolelevel<=50 then
					addsilver = 75000
				elseif rolelevel<=80 then
					addsilver = 150000
				elseif rolelevel<=100 then
					addsilver = 300000
				end

				role.SetRoleScriptData(RoleID, 1, RoleDataType["VoteDate"], curdate)
				role.AddRoleSilver(MapID, InstanceID, RoleID, addsilver, 102)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3242)			--�ź������ɹ������x����x��Ǯ���뿪ʼͶƱ
				msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
				msg.AddMsgEvent(MsgID, 21,	5)				--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+1)	--������
				msg.AddMsgEvent(MsgID, 21,	6)				--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+2)	--������
				msg.AddMsgEvent(MsgID, 21,	7)				--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+3)	--������
				msg.AddMsgEvent(MsgID, 21,	8)				--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	3201+SupportGroupID*4+4)	--������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	end
end

aux.RegisterCreatureEvent(4900473, 7, "ZuQiuBaoBei_OnTalk")
aux.RegisterCreatureEvent(4900474, 7, "ZuQiuBaoBei_OnTalk")
aux.RegisterCreatureEvent(4900475, 7, "ZuQiuBaoBei_OnTalk")
aux.RegisterCreatureEvent(4900476, 7, "ZuQiuBaoBei_OnTalk")
aux.RegisterCreatureEvent(4900477, 7, "ZuQiuBaoBei_OnTalk")
aux.RegisterCreatureEvent(4900478, 7, "ZuQiuBaoBei_OnTalk")
aux.RegisterCreatureEvent(4900479, 7, "ZuQiuBaoBei_OnTalk")
aux.RegisterCreatureEvent(4900480, 7, "ZuQiuBaoBei_OnTalk")

function WC_ShuoMingRen_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 3243)			--����˵��
		msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
		msg.AddMsgEvent(MsgID, 21,	4)				--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3244)	--��ȡ����
		msg.AddMsgEvent(MsgID, 21,	5)				--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3272)	--�һ��ھ�ӡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 5 then

		local i = role.GetRoleItemNum(RoleID, 3304108)

		if i<1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3273)			--����˵��
			msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		else

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3274)			--����˵��
			msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
			msg.AddMsgEvent(MsgID, 21, 6)				--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21)				--ȷ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		end

	elseif TalkIndex == 6 then

		local i = role.GetRoleItemNum(RoleID, 3304108)
		role.RemoveFromRole(MapID, InstanceID, RoleID, 3304108, i, 420)

		local NeedRoom = math.floor(i/9999)+1

		if NeedRoom>1 then

			for i=1,NeedRoom-1 do
				role.AddRoleItem(MapID, InstanceID, RoleID, 3304102, 9999, -1, 8, 420)
			end

		end

		role.AddRoleItem(MapID, InstanceID, RoleID, 3304102, i%9999, -1, 8, 420)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 3275)			--����˵��
		msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then

		local Champion = act.GetActScriptData(145,1,34)
		local SupportTeam = role.GetRoleScriptData(RoleID, 1, RoleDataType["SupportTeam"])

		if Champion == 0 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3245)			--���籭��δ�������޷���ȡ
			msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif SupportTeam == 0 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3246)			--��û��ͶƱ֧�ֹ��κ����
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif Champion==SupportTeam then

			local VoteSum = act.GetActScriptData(145, 1, 33)
			local TempVote = act.GetActScriptData(145, 1, Champion)
			local VoteNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["VoteNum"])

			local RewNum = math.floor(VoteSum/TempVote*VoteNum*0.8)

			if RewNum<VoteNum then
				RewNum = VoteNum
			end

			local NeedRoom = math.floor(RewNum/9999)+1
			local FreeSize = role.GetBagFreeSize(RoleID)

			if FreeSize>=NeedRoom then

				if NeedRoom>1 then

					for i=1,NeedRoom-1 do
						role.AddRoleItem(MapID, InstanceID, RoleID, 3304102, 9999, -1, 8, 420)
					end

				end

				role.AddRoleItem(MapID, InstanceID, RoleID, 3304102, RewNum%9999, -1, 8, 420)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3247)			--��ȡ�����ɹ�
				msg.AddMsgEvent(MsgID, 9, RewNum)
				msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				role.SetRoleScriptData(RoleID, 1, RoleDataType["SupportTeam"], 0)

			else

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3248)			--���ҿռ䲻��
				msg.AddMsgEvent(MsgID, 9, NeedRoom)
				msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
				msg.AddMsgEvent(MsgID, 21,	4)				--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	SupportGroupID*4+1)	--��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end


		elseif Champion~=SupportTeam then

			local VoteNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["VoteNum"])
			local VoteSum = act.GetActScriptData(145, 1, 33)
			local TempVote = VoteSum - act.GetActScriptData(145, 1, Champion)

			local RewNum = math.floor(VoteSum/TempVote*VoteNum*0.2*10)


			role.AddRoleGiftPoints(MapID, InstanceID, RoleID, RewNum)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["SupportTeam"], 0)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3249)			--��ȡ�����ɹ�
			msg.AddMsgEvent(MsgID, 9, RewNum)
			msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		end
	end
end

aux.RegisterCreatureEvent(4900481, 7, "WC_ShuoMingRen_OnTalk")

function I3304109_WorldCup(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	act.SetActScriptData(145, 1, 34, TypeID-3304108)
	act.SaveActScriptData(145)
end

aux.RegisterItemEvent(3304109, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304110, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304111, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304112, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304113, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304114, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304115, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304116, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304117, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304118, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304119, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304120, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304121, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304122, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304123, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304124, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304125, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304126, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304127, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304128, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304129, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304130, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304131, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304132, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304133, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304134, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304135, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304136, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304137, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304138, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304139, 1, "I3304109_WorldCup")
aux.RegisterItemEvent(3304140, 1, "I3304109_WorldCup")

function WC_JiuBa_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local MatchList = {}

	MatchList[611] = {starthour=-1, startmin=0}
	MatchList[612] = {starthour=22, startmin=0}
	MatchList[613] = {starthour=22, startmin=0}
	MatchList[614] = {starthour=19, startmin=30}
	MatchList[615] = {starthour=22, startmin=0}
	MatchList[616] = {starthour=22, startmin=0}
	MatchList[617] = {starthour=19, startmin=30}
	MatchList[618] = {starthour=22, startmin=0}
	MatchList[619] = {starthour=19, startmin=30}
	MatchList[620] = {starthour=19, startmin=30}
	MatchList[621] = {starthour=19, startmin=30}
	MatchList[622] = {starthour=22, startmin=0}
	MatchList[623] = {starthour=22, startmin=0}
	MatchList[624] = {starthour=22, startmin=0}
	MatchList[625] = {starthour=22, startmin=0}
	MatchList[626] = {starthour=22, startmin=0}
	MatchList[627] = {starthour=22, startmin=0}
	MatchList[628] = {starthour=22, startmin=0}
	MatchList[629] = {starthour=22, startmin=0}
	MatchList[630] = {starthour=-1, startmin=0}
	MatchList[701] = {starthour=-1, startmin=0}
	MatchList[702] = {starthour=22, startmin=0}
	MatchList[703] = {starthour=22, startmin=0}
	MatchList[704] = {starthour=-1, startmin=0}
	MatchList[705] = {starthour=-1, startmin=0}
	MatchList[706] = {starthour=-1, startmin=0}
	MatchList[707] = {starthour=2, startmin=30}
	MatchList[708] = {starthour=2, startmin=30}
	MatchList[709] = {starthour=-1, startmin=0}
	MatchList[710] = {starthour=-1, startmin=0}
	MatchList[711] = {starthour=2, startmin=30}
	MatchList[712] = {starthour=2, startmin=30}
	MatchList[713] = {starthour=-1, startmin=0}
	MatchList[714] = {starthour=-1, startmin=0}

	if TalkIndex==-1 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 3252)			--����˵��
		msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
		msg.AddMsgEvent(MsgID, 21, 4)				--��ť
		msg.AddMsgEvent(MsgID, 1, 3253)				--�����ˮ
		msg.AddMsgEvent(MsgID, 21, 5)				--��ť
		msg.AddMsgEvent(MsgID, 1, 3254)				--�����ˮ
		msg.AddMsgEvent(MsgID, 21, 6)				--��ť
		msg.AddMsgEvent(MsgID, 1, 3255)				--�����ˮ
		msg.AddMsgEvent(MsgID, 21, 7)				--��ť
		msg.AddMsgEvent(MsgID, 1, 3256)				--�����ˮ
		msg.AddMsgEvent(MsgID, 21, 8)				--��ť
		msg.AddMsgEvent(MsgID, 1, 3257)				--�����ˮ
		msg.AddMsgEvent(MsgID, 21, 9)				--��ť
		msg.AddMsgEvent(MsgID, 1, 3258)				--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex>=4 and TalkIndex<=8 then

		local LastBuyWine = role.GetRoleScriptData(RoleID, 1, RoleDataType["LastBuyWine"])
		local curday = tonumber(os.date("%d"))
		local curmonth = tonumber(os.date("%m"))
		local curhour = tonumber(os.date("%H"))
		local curmin = tonumber(os.date("%M"))

		local starthour = MatchList[curmonth*100+curday].starthour
		local startmin = MatchList[curmonth*100+curday].startmin

		local nextstarthour = MatchList[curmonth*100+curday+1].starthour

		if starthour==-1 and nextstarthour~=2 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3259)			--����û�б���
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif starthour>17 and (curhour<=17 or curhour*60+curmin-starthour*60-startmin>=50) then

			if curhour<=17 then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3260)			--û�е�ʱ��
				msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif curhour*60+curmin-starthour*60-startmin>=50 then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3261)			--ʱ���Ѿ�����
				msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end

		elseif curhour*60+curmin>=200 and curhour<=17 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3261)			--ʱ���Ѿ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif curhour*60+curmin<200 and starthour~=2 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3259)			--����û�б���
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif curhour>17 and starthour<=17 and nextstarthour~=2 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3260)			--û�е�ʱ��
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		else

			if (curmonth*30+curday)*24+curhour-LastBuyWine>=12 then
				role.SetRoleScriptData(RoleID, 1, RoleDataType["LastBuyWine"], (curmonth*30+curday)*24+curhour)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Wine_Fee_A"], 0)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Wine_Fee_B"], 0)
			end

			local FreeSize = role.GetBagFreeSize(RoleID)

			if FreeSize<1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3262)			--���ҿռ䲻��
				msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else

				local ticketnum = role.GetRoleItemNum(RoleID, 3304108)
				local needticket = {}
				needticket[4] = 1
				needticket[5] = 2
				needticket[6] = 5
				needticket[7] = 10
				needticket[8] = 20

				if ticketnum<needticket[TalkIndex] then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3263)			--��Ǯ����
					msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else

					role.RemoveFromRole(MapID, InstanceID, RoleID, 3304108, needticket[TalkIndex], 420)
					role.AddRoleItem(MapID, InstanceID, RoleID, 3304099+TalkIndex, 1, -1, 8, 420)
					local curfee = role.GetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483)
					role.SetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483, curfee+needticket[TalkIndex])

					local team_sum = act.GetActScriptData(145, 1, 35+TargetTypeID-4900483)
					local total_sum = act.GetActScriptData(145, 1, 37)

					act.SetActScriptData(145, 1, 35+TargetTypeID-4900483, team_sum+needticket[TalkIndex])
					act.SetActScriptData(145, 1, 37, total_sum+needticket[TalkIndex])
					act.SaveActScriptData(145)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3264)			--�����ˮ�ɹ�
					msg.AddMsgEvent(MsgID, 9, curfee+needticket[TalkIndex])
					msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
					msg.AddMsgEvent(MsgID, 21, 4)				--��ť
					msg.AddMsgEvent(MsgID, 1, 3253)				--�����ˮ
					msg.AddMsgEvent(MsgID, 21, 5)				--��ť
					msg.AddMsgEvent(MsgID, 1, 3254)				--�����ˮ
					msg.AddMsgEvent(MsgID, 21, 6)				--��ť
					msg.AddMsgEvent(MsgID, 1, 3255)				--�����ˮ
					msg.AddMsgEvent(MsgID, 21, 7)				--��ť
					msg.AddMsgEvent(MsgID, 1, 3256)				--�����ˮ
					msg.AddMsgEvent(MsgID, 21, 8)				--��ť
					msg.AddMsgEvent(MsgID, 1, 3257)				--�����ˮ
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end

	elseif TalkIndex==9 then

		if WorldCup_Today_Team == -1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3265)			--�����޷���ȡ����
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif WorldCup_Today_Team == 2 then
			local cost_num = role.GetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483)

			if cost_num == 0 then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3266)			--û�����ѹ�
				msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else

				local RewNum = math.floor(cost_num * 0.97)

				local NeedRoom = math.floor(RewNum/999)+1
				local FreeSize = role.GetBagFreeSize(RoleID)

				if FreeSize>=NeedRoom then

					role.SetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483, 0)

					if NeedRoom>1 then

						for i=1,NeedRoom-1 do
							role.AddRoleItem(MapID, InstanceID, RoleID, 3304108, 999, -1, 8, 420)
						end

					end

					role.AddRoleItem(MapID, InstanceID, RoleID, 3304108, RewNum%999, -1, 8, 420)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3271)			--��ȡ�����ɹ�
					msg.AddMsgEvent(MsgID, 9, RewNum)
					msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					role.GetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483, 0)

				else

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3248)			--���ҿռ䲻��
					msg.AddMsgEvent(MsgID, 9, NeedRoom+temp)
					msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end

		elseif WorldCup_Today_Team == TargetTypeID-4900483 then

			local cost_num = role.GetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483)

			if cost_num == 0 then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3266)			--û�����ѹ�
				msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else

				local cost_sum = act.GetActScriptData(145, 1, 35+TargetTypeID-4900483)
				local total_sum = act.GetActScriptData(145, 1, 37)

				local RewNum = math.floor(total_sum/cost_sum * cost_num * 0.97)

				if RewNum<cost_num then
					RewNum = cost_num
				end

				local NeedRoom = math.floor(RewNum/999)+1
				local FreeSize = role.GetBagFreeSize(RoleID)

				if FreeSize>=NeedRoom then

					role.SetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483, 0)

					if NeedRoom>1 then

						for i=1,NeedRoom-1 do
							role.AddRoleItem(MapID, InstanceID, RoleID, 3304108, 999, -1, 8, 420)
						end

					end

					role.AddRoleItem(MapID, InstanceID, RoleID, 3304108, RewNum%999, -1, 8, 420)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3267)			--��ȡ�����ɹ�
					msg.AddMsgEvent(MsgID, 9, RewNum)
					msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					role.GetRoleScriptData(RoleID, 1, 135+TargetTypeID-4900483, 0)

				else

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3248)			--���ҿռ䲻��
					msg.AddMsgEvent(MsgID, 9, NeedRoom+temp)
					msg.AddMsgEvent(MsgID, 24,	TargetID)		--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end

		elseif WorldCup_Today_Team ~= TargetTypeID-4900483 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3268)			--ƽ��/���ˣ�û����
			msg.AddMsgEvent(MsgID, 24, TargetID)		--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

aux.RegisterCreatureEvent(4900483, 7, "WC_JiuBa_OnTalk")
aux.RegisterCreatureEvent(4900484, 7, "WC_JiuBa_OnTalk")

function I3304141_WorldCup(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��

	if TypeID == 3304141 then
		WorldCup_Today_Team = 0
	elseif TypeID == 3304142 then
		WorldCup_Today_Team = 1
	else
		WorldCup_Today_Team = 2
	end

end

aux.RegisterItemEvent(3304141, 1, "I3304141_WorldCup")
aux.RegisterItemEvent(3304142, 1, "I3304141_WorldCup")
aux.RegisterItemEvent(3304151, 1, "I3304141_WorldCup")
