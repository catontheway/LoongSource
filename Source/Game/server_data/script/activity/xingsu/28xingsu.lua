--�̶������ʮ��������¡��

--�������������
math.randomseed(aux.TimeGetTime())

--�칬�ǹ�

tiangongxingguan={}
tiangongxingguan[3600020]={id=-1, IsDead=true, MapCrc=3017298127, x=0, y=0, z=0}
tiangongxingguan[3600021]={id=-1, IsDead=true, MapCrc=3017298383, x=0, y=0, z=0}
tiangongxingguan[3600022]={id=-1, IsDead=true, MapCrc=3017299663, x=0, y=0, z=0}
tiangongxingguan[3600023]={id=-1, IsDead=true, MapCrc=3017299919, x=0, y=0, z=0}
tiangongxingguan[3600024]={id=-1, IsDead=true, MapCrc=3017299151, x=0, y=0, z=0}
tiangongxingguan[3600025]={id=-1, IsDead=true, MapCrc=3017299407, x=0, y=0, z=0}

--��ʮ������table
act1_Creature = {}
act1_Creature[1500001] = {id=-1, IsDead=true, MapCrc=3017298127, x=0, y=0, z=0, Rate=100, BuffID=-1}
act1_Creature[1500002] = {id=-1, IsDead=true, MapCrc=3017298127, x=0, y=0, z=0, Rate=100, BuffID=5000101}
act1_Creature[1500003] = {id=-1, IsDead=true, MapCrc=3017298127, x=0, y=0, z=0, Rate=50, BuffID=-1}
act1_Creature[1500004] = {id=-1, IsDead=true, MapCrc=3017298127, x=0, y=0, z=0, Rate=50, BuffID=5000201}
act1_Creature[1500005] = {id=-1, IsDead=true, MapCrc=3017298383, x=0, y=0, z=0, Rate=100, BuffID=-1}
act1_Creature[1500006] = {id=-1, IsDead=true, MapCrc=3017298383, x=0, y=0, z=0, Rate=100, BuffID=5000301}
act1_Creature[1500007] = {id=-1, IsDead=true, MapCrc=3017298383, x=0, y=0, z=0, Rate=50, BuffID=-1}
act1_Creature[1500008] = {id=-1, IsDead=true, MapCrc=3017298383, x=0, y=0, z=0, Rate=50, BuffID=5000401}
act1_Creature[1500009] = {id=-1, IsDead=true, MapCrc=3017299663, x=0, y=0, z=0, Rate=100, BuffID=-1}
act1_Creature[1500010] = {id=-1, IsDead=true, MapCrc=3017299663, x=0, y=0, z=0, Rate=100, BuffID=5000501}
act1_Creature[1500011] = {id=-1, IsDead=true, MapCrc=3017299663, x=0, y=0, z=0, Rate=50, BuffID=-1}
act1_Creature[1500012] = {id=-1, IsDead=true, MapCrc=3017299663, x=0, y=0, z=0, Rate=50, BuffID=-1}
act1_Creature[1500013] = {id=-1, IsDead=true, MapCrc=3017299663, x=0, y=0, z=0, Rate=50, BuffID=5000601}
act1_Creature[1500014] = {id=-1, IsDead=true, MapCrc=3017299919, x=0, y=0, z=0, Rate=100, BuffID=5000701}
act1_Creature[1500015] = {id=-1, IsDead=true, MapCrc=3017299919, x=0, y=0, z=0, Rate=100, BuffID=-1}
act1_Creature[1500016] = {id=-1, IsDead=true, MapCrc=3017299919, x=0, y=0, z=0, Rate=50, BuffID=-1}
act1_Creature[1500017] = {id=-1, IsDead=true, MapCrc=3017299919, x=0, y=0, z=0, Rate=50, BuffID=5000801}
act1_Creature[1500018] = {id=-1, IsDead=true, MapCrc=3017299919, x=0, y=0, z=0, Rate=50, BuffID=5000901}
act1_Creature[1500019] = {id=-1, IsDead=true, MapCrc=3017299151, x=0, y=0, z=0, Rate=100, BuffID=-1}
act1_Creature[1500020] = {id=-1, IsDead=true, MapCrc=3017299151, x=0, y=0, z=0, Rate=100, BuffID=5001001}
act1_Creature[1500021] = {id=-1, IsDead=true, MapCrc=3017299151, x=0, y=0, z=0, Rate=50, BuffID=-1}
act1_Creature[1500022] = {id=-1, IsDead=true, MapCrc=3017299151, x=0, y=0, z=0, Rate=50, BuffID=5001101}
act1_Creature[1500023] = {id=-1, IsDead=true, MapCrc=3017299151, x=0, y=0, z=0, Rate=50, BuffID=5001201}
act1_Creature[1500024] = {id=-1, IsDead=true, MapCrc=3017299407, x=0, y=0, z=0, Rate=100, BuffID=-1}
act1_Creature[1500025] = {id=-1, IsDead=true, MapCrc=3017299407, x=0, y=0, z=0, Rate=100, BuffID=5001301}
act1_Creature[1500026] = {id=-1, IsDead=true, MapCrc=3017299407, x=0, y=0, z=0, Rate=50, BuffID=-1}
act1_Creature[1500027] = {id=-1, IsDead=true, MapCrc=3017299407, x=0, y=0, z=0, Rate=50, BuffID=5001401}
act1_Creature[1500028] = {id=-1, IsDead=true, MapCrc=3017299407, x=0, y=0, z=0, Rate=50, BuffID=5001501}



--���TypeIDΪK�����޼�¼
function Clear_tiangongxingguan(k)
	tiangongxingguan[k].id = -1
	tiangongxingguan[k].IsDead = true
	tiangongxingguan[k].x = 0
	tiangongxingguan[k].y = 0
	tiangongxingguan[k].z = 0
end

--��ʼ��tiangongxingguan
function Init_tiangongxingguan()
	for k in pairs(tiangongxingguan) do
		Clear_tiangongxingguan(k)
	end
end

--���ʼ�¼�
function act1_OnStart(actID)
	--��ʼ��tiangongxingguan
	Init_tiangongxingguan()
	--֪ͨȫ�����ʼ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 5)
	msg.DispatchWorldMsgEvent(MsgID)
	for k = 3600020 ,3600025 do
			--���һ��ˢ�ֵ�
		if  tiangongxingguan[k].id == -1 then
			local MapCrc = tiangongxingguan[k].MapCrc
			local randPoint = math.random(500, 520)
			local x = map_list[MapCrc].map[randPoint].x
			local y = map_list[MapCrc].map[randPoint].y
			local z = map_list[MapCrc].map[randPoint].z
			--�����칬�ǹ�
			local success = map.MapCreateCreature(MapCrc, -1, k, x, y, z)
			--�����ɹ�
			if success~=4294967295 then
				--����tiangongxingguan
				tiangongxingguan[k].id = success
				tiangongxingguan[k].IsDead = false
				tiangongxingguan[k].x = x
				tiangongxingguan[k].y = y
				tiangongxingguan[k].z = z
			end
		end
	end
end

--ע��û�����ʱ��
aux.RegisterActEvent(1, 2, "act1_OnStart")
aux.RegisterActEvent(2, 2, "act1_OnStart")
aux.RegisterActEvent(3, 2, "act1_OnStart")
aux.RegisterActEvent(4, 2, "act1_OnStart")
--aux.RegisterActEvent(86, 2, "act1_OnStart")
aux.RegisterActEvent(89, 2, "act1_OnStart")
aux.RegisterActEvent(90, 2, "act1_OnStart")


function xingguan_OnTalk(MapID, InstanceID, NPCID, NPCTypeID, TargetID, Index)
		if Index == -1 then
		  local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	422)	---�Ի�����ʾ����Ҫ��ս�칬���ޣ�������������Ҫ�ﵽ3�ˡ����Ƿ��Ѿ�����׼������
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
				msg.AddMsgEvent(MsgID, 71, 423)--��ʾ������������3��
				msg.DispatchRoleMsgEvent(TargetID,MsgID)
				return -1
			else
				local localA, localB ,localC = unit.GetPosition(MapID, InstanceID, TargetID)
				--����
				if MapID == 3017298127 then
					local monsterid = 1500000 + math.random(1,4)
					map.MapCreateCreature(3017298127, InstanceID, monsterid, localA+2, localB, localC+12)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA+15, localB, localC+15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA+5, localB, localC-15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA-15, localB, localC+15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA-5, localB, localC-15)
					map.MapCreateCreature(3017298127, InstanceID, 1510015, localA+17, localB, localC+17)
				--����
				elseif MapID == 3017298383 then
					local monsterid =  1500000 + math.random(5,8)
					map.MapCreateCreature(3017298383, InstanceID, monsterid, localA+2, localB, localC+12)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA+15, localB, localC+15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA+5, localB, localC-15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA-15, localB, localC+15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA-5, localB, localC-15)
					map.MapCreateCreature(3017298383, InstanceID, 1510016, localA+17, localB, localC+17)
				--��¹
				elseif MapID == 3017299663 then
					local monsterid = 1500000 +  math.random(9,13)
					map.MapCreateCreature(3017299663, InstanceID, monsterid, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299663, InstanceID, 1510017, localA+17, localB, localC+17)
				--��ˮ
				elseif MapID == 3017299919 then
					local monsterid =  1500000 + math.random(14,18)
					map.MapCreateCreature(3017299919, InstanceID, monsterid, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299919, InstanceID, 1510018, localA+17, localB, localC+17)
				--����m06
				elseif MapID == 3017299151 then
					local monsterid =  1500000 + math.random(19,23)
					map.MapCreateCreature(3017299151, InstanceID, monsterid, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299151, InstanceID, 1510019, localA+17, localB, localC+17)
				--����
				elseif MapID == 3017299407 then
					local monsterid =  1500000 + math.random(24,28)
					map.MapCreateCreature(3017299407, InstanceID, monsterid, localA+2, localB, localC+12)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA+15, localB, localC+15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA+5, localB, localC-15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA-15, localB, localC+15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA-5, localB, localC-15)
					map.MapCreateCreature(3017299407, InstanceID, 1510020, localA+17, localB, localC+17)
				end
			end
			map.MapDeleteCreature(MapID, InstanceID, NPCID)
		end
	return
end



aux.RegisterCreatureEvent(3600020, 7, "xingguan_OnTalk")
aux.RegisterCreatureEvent(3600021, 7, "xingguan_OnTalk")
aux.RegisterCreatureEvent(3600022, 7, "xingguan_OnTalk")
aux.RegisterCreatureEvent(3600023, 7, "xingguan_OnTalk")
aux.RegisterCreatureEvent(3600024, 7, "xingguan_OnTalk")
aux.RegisterCreatureEvent(3600025, 7, "xingguan_OnTalk")



