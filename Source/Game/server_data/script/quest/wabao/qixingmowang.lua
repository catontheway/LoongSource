

--������ħ���Ի�����ʾ�Ի����ݣ���Ҫ��ս���������ħ������������������������Ҫ�ﵽ3�ˡ����Ƿ��Ѿ�����׼������

function n4500003_OnTalk(MapID, InstanceID, NPCID, NPCTypeID, TargetID, Index)
		if Index == -1 then
		  local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	417)	---�Ի�����ʾ����Ҫ��ս���������ħ������������������������Ҫ�ﵽ3�ˡ����Ƿ��Ѿ�����׼������
			msg.AddMsgEvent(MsgID, 24,	NPCID)	--npcid
		    msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	21)				--��ȷ����
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)
			msg.DispatchRoleMsgEvent(TargetID, MsgID)
		end
		if Index == 4 then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
			local Man_InTeam = 0
			local Role = {}
				   Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
				for i=1, 6 do
					if Role[i] and Role[i]~= 4294967295 then
					Man_InTeam = Man_InTeam + 1
					end
				end
			--����ˢ�����������������ﵽ3��
			if Man_InTeam < 3  then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 411)--��ʾ������������3��
				msg.DispatchRoleMsgEvent(TargetID,MsgID)
				return -1
				--[[local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,  1)	--Ҫ��ս���������ħ������������������������Ҫ�ﵽ3�ˣ�	��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				msg.AddMsgEvent(MsgID, 100, 411)
				msg.AddMsgEvent(MsgID, 6, TempMapID)
				msg.DispatchWorldMsgEvent(MsgID)]]
			else
				local localA, localB ,localC = unit.GetPosition(MapID, InstanceID, TargetID)
				if MapID == 3017298127 then
					map.MapCreateCreature(3017298127, InstanceID, 1510072, localA+2, localB, localC+12)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA+15, localB, localC+15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA+5, localB, localC-15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA-15, localB, localC+15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA-5, localB, localC-15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA+17, localB, localC+17)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA-7, localB, localC-17)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA+17, localB, localC-17)
				--����
				elseif MapID == 3017298383 then
					map.MapCreateCreature(3017298383, InstanceID, 1510073, localA+2, localB, localC+12)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA+15, localB, localC+15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA+5, localB, localC-15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA-15, localB, localC+15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA-5, localB, localC-15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA+17, localB, localC+17)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA-7, localB, localC-17)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA+17, localB, localC-17)
				--��¹
				elseif MapID == 3017299663 then
					map.MapCreateCreature(3017299663, InstanceID, 1510074, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA+17, localB, localC+17)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA-7, localB, localC-17)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA+17, localB, localC-17)
					--��ˮ
				elseif MapID == 3017299919 then
					map.MapCreateCreature(3017299919, InstanceID, 1510075, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA+17, localB, localC+17)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA-7, localB, localC-17)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA+17, localB, localC-17)
				--����m06
				elseif MapID == 3017299151 then
					map.MapCreateCreature(3017299151, InstanceID, 1510076, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA+17, localB, localC+17)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA-7, localB, localC-17)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA+17, localB, localC-17)
				--����m06_1
				elseif MapID == 1146339967 then
					map.MapCreateCreature(1146339967, InstanceID, 1510076, localA+2, localB, localC+12)
					map.MapCreateCreature(1146339967, InstanceID, 1510019, localA+15, localB, localC+15)
					map.MapCreateCreature(1146339967, InstanceID, 1510019, localA+5, localB, localC-15)
					map.MapCreateCreature(1146339967, InstanceID, 1510019, localA-15, localB, localC+15)
					map.MapCreateCreature(1146339967, InstanceID, 1510019, localA-5, localB, localC-15)
					map.MapCreateCreature(1146339967, InstanceID, 1510019, localA+17, localB, localC+17)
					map.MapCreateCreature(1146339967, InstanceID, 1510019, localA-7, localB, localC-17)
					map.MapCreateCreature(1146339967, InstanceID, 1510019, localA+17, localB, localC-17)
				--����
				elseif MapID == 3017299407 then
					map.MapCreateCreature(3017299407, InstanceID, 1510077, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA+17, localB, localC+17)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA-7, localB, localC-17)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA+17, localB, localC-17)
				end
			end
			map.MapDeleteCreature(MapID, InstanceID, NPCID)
		end
	return
end

aux.RegisterCreatureEvent(4500003, 7, "n4500003_OnTalk")



