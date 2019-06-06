
Quest_WaBao_Pos = {}
Quest_WaBao_Pos[1] = {x=2074, z=1661}
Quest_WaBao_Pos[2] = {x=2013, z=2039}
Quest_WaBao_Pos[3] = {x=2330, z=1845}
Quest_WaBao_Pos[4] = {x=1748, z=3141}
Quest_WaBao_Pos[5] = {x=1509, z=3184}
Quest_WaBao_Pos[6] = {x=1349, z=3041}
Quest_WaBao_Pos[7] = {x=1322, z=2990}
Quest_WaBao_Pos[8] = {x=1744, z=2635}
Quest_WaBao_Pos[9] = {x=1554, z=2149}
Quest_WaBao_Pos[10] = {x=1323, z=2160}
Quest_WaBao_Pos[11] = {x=1828, z=1479}
Quest_WaBao_Pos[12] = {x=1026, z=1084}
Quest_WaBao_Pos[13] = {x=1092, z=1008}
Quest_WaBao_Pos[14] = {x=2489, z=948}
Quest_WaBao_Pos[15] = {x=2713, z=775}
Quest_WaBao_Pos[16] = {x=2637, z=996}
Quest_WaBao_Pos[17] = {x=2272, z=1173}
Quest_WaBao_Pos[18] = {x=3052, z=1472}
Quest_WaBao_Pos[19] = {x=2970, z=1413}
Quest_WaBao_Pos[20] = {x=2411, z=1930}

--��������������˶Ի�
function n3011065_OnTalk(MapID, InstanceID, NPCID, NPCTypeID, TargetID, Index)
	--��������Ƿ��ǺϷ��ģ�ֻ��ʹ��̰��ͼ����Ҽ�������ǺϷ���)
	local TriggerRoleID = cre.GetCreatureScriptData(MapID, InstanceID, NPCID, 1, 1)
	--Flag 0:�öԻ���Ҳ��ǺϷ��� 1���Ϸ���ң����Ҿ���ʹ�ñ�ͼ�� 2���Ϸ���ң���ʹ�ñ�ͼ�ߵĶ���
	local Flag = 0
	if TargetID == TriggerRoleID then
		Flag = 1
	else
	  local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
	  local TeamMemID = {}
		TeamMemID[1], TeamMemID[2], TeamMemID[3], TeamMemID[4], TeamMemID[5], TeamMemID[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
	    if TeamMemID[i] == TriggerRoleID then
	    	Flag = 2
	    	break
	    end
    end
end



	if Flag == 0 then
		return
	end

	if Flag == 1 then

		if Index == -1 then
		  --�Ի�����ʾ�������Ƿ�ȷ��ǰ�������������������������ѶȽϸߣ���������֯���˶��飬һ����ս��������ȷ��/ȡ������
		  local MsgID = msg.BeginMsgEvent()
		  msg.AddMsgEvent(MsgID, 20, 57)	--�����Ƿ�ȷ��ǰ�������������
			msg.AddMsgEvent(MsgID, 24,	NPCID)	--npcid
		  msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	21)				--��ȷ����
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)				--��ȡ����
			msg.DispatchRoleMsgEvent(TargetID, MsgID)
		end
		if Index == 4 then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
			local Man_InTeam = 0
			local TeamMemID = {}
				   TeamMemID[1], TeamMemID[2], TeamMemID[3], TeamMemID[4], TeamMemID[5], TeamMemID[6] = role.GetRoleTeamMemID(TeamID)
				for i=1, 6 do
					if  TeamMemID[i] and TeamMemID[i]~= 4294967295 then
						Man_InTeam = Man_InTeam + 1
					end
				end
				--���ƽ����������������������������ﵽ3��
			if Man_InTeam < 3  then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 414)--��ʾ������������3��
				msg.DispatchRoleMsgEvent(TargetID,MsgID)
				return -1
			else
				role.SetRoleScriptData(TriggerRoleID, 1, RoleDataType["LongMaiXunBao_Do"], 1)
				--����ʾ�������
				--if TeamID == 4294967295 then
					--��ҽ����������������2092184116��=17300  Y=225  Z=9250
					--role.RoleGotoNewMap(MapID, InstanceID, TargetID, 2092184116, 346, 20, 185)
					--return
				--else
				local TeamMemID = {}
					TeamMemID[1], TeamMemID[2], TeamMemID[3], TeamMemID[4], TeamMemID[5], TeamMemID[6] = role.GetRoleTeamMemID(TeamID)
					for i=1, 6 do
						if TeamMemID[i] ~= 4294967295 and TeamMemID[i] ~= TriggerRoleID then
							local NPCx,NPCy,NPCz = role.GetRoleScriptData(TriggerRoleID, 3, RoleDataType["LongMaiXunBao_NPC_X"], RoleDataType["LongMaiXunBao_NPC_Y"], RoleDataType["LongMaiXunBao_NPC_Z"])
							  --�����Ķ�����ǰ���������Ѱ������ǰ��[��ͼ��##������##]Ѱ��������������ˣ�������ͬ�μ���ս����
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 65)
							msg.AddMsgEvent(MsgID, 6, MapID)
							msg.AddMsgEvent(MsgID, 9, NPCx)
							msg.AddMsgEvent(MsgID, 9, NPCz)
							msg.AddMsgEvent(MsgID, 21, 4)			--ȷ����ť
							msg.AddMsgEvent(MsgID, 1, 21)			--��ȷ����
							msg.DispatchRoleMsgEvent(TeamMemID[i], MsgID)
						end
				    end
			    end
			    --��ҽ����������������2092184116��=17300  Y=225  Z=9250
			    role.RoleGotoNewMap(MapID, InstanceID, TargetID, 2092184116, 346, 20, 185)
				map.MapDeleteCreature(MapID, InstanceID, NPCID)
			end
		end
		return
	end

	if Flag == 2 then
		--�ж�ʹ�ñ�ͼ������Ƿ�ͬ����븱��
		local Agree = role.GetRoleScriptData(TriggerRoleID, 1, RoleDataType["LongMaiXunBao_Do"])
		if Agree == 1 then
		else
			return
		end
		--�������Ի�����ʾ�������Ƿ�ȷ��ǰ���������������ȷ��/ȡ������
		if Index == -1 then
		  local MsgID = msg.BeginMsgEvent()
		  msg.AddMsgEvent(MsgID, 20, 	57)			--�����Ƿ�ȷ��ǰ�������������
		  msg.AddMsgEvent(MsgID, 24,	NPCID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	21)				--��ȷ����
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)				--��ȡ����
			msg.DispatchRoleMsgEvent(TargetID, MsgID)
		end
		if Index == 4 then
			--��ҽ����������������2092184116��
			role.RoleGotoNewMap(MapID, InstanceID, TargetID, 2092184116, 346, 20, 185)
		end
	end



--�뿪������������Ľű�
function a02_LeaveMap(MapID, InstanceID, TargetID)
	local bool = role.GetRoleScriptData(TargetID, 1, RoleDataType["LongMaiXunBao_Do"])
	if bool == 1 then
		role.SetRoleScriptData(TargetID, 1, RoleDataType["LongMaiXunBao_Do"], 0)
	end
end

-- ע��
aux.RegisterCreatureEvent(3011065, 7, "n3011065_OnTalk")
aux.RegisterMapEvent("a02", 3, "a02_LeaveMap")
