--��������ʼʱ�����Ľű�
Arena = {}                    --������¼��������ͼ������Ӧ�ľ�����������Լ�����˫��ʣ�������
PlayerStillInArena = {}              --������¼���뾺������û���뿪�����
PlayerTransToArena = {}          --������¼�����ͽ������������
Alive_Arena = {}              --������¼�������д������
Totle_Arena = {}               --������¼�����������е����
ArenaCreature_Timer = {}           --������¼�������еļ�ʱ���������ڵĸ���ID������ID��
	--[[ArenaCreature_Timer[creatureID]= {TimerTypeID}
	   TimerTypeIDö��ֵ
	   1 1���ӵĿ��ż�ʱ��
	   2 10���ӵĵ���ʱ��ʱ��
	   3 30��Ĺ��ɼ�ʱ������15��ģ�
	   4 15������Լ�ʱ��������15��Ϳ��ŵļ�ʱ����
	   5 10���ӵĽ����ʱ�����ڿ��ź�ˢ�����ӿ��أ����û�����򲻻ᴥ��Ч��
	   6 15��Ĵ��ͼ�ʱ��
	   7 30������ս��
	   8 30��ķ�����ּ�ʱ
	   9 15�뱨ʱ
	   10 15���ͷ��ڴ�
	   ]]
BeInArena = {}                --��¼������Ƿ�ϵͳ���ͽ���������������ǣ����䴫�������ܾ�����.�뿪������ʱ����ֵ������
MapIDBeforeEnterArena = {}    --��¼��ҽ��뾺������ǰ�ĵ�ͼID����Ҫ������뿪ʱɾ����
BattleEnd_Arena = {}          --��¼�ø���������Ƿ��Ѿ����㣨�����˲��뿪����Ҳ��ᱻ�۷�)
BattleBegin_Arena = {}        --��¼�ø����Ƿ��Ѿ��򿪴��ţ����˴��Ų��ܿ۷֣������˳����۷�)
KillPeople_Arena = {}         --��¼����ھ�������ɱ��������
RankPoint_Arena = {}          --��¼˫������ѫ�ȼ�
Arena_Minutes = {}            --��¼��Ϸ���е����ڵķ�����*2
Before_Arena = {}             --������¼�����������е����ԭ����λ��

function a06_ArenaOnStart(MapID, InstanceID, RoomID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6)

    Before_Arena[InstanceID] = {}               --������¼�����������е����
	Before_Arena[InstanceID][1] = RoleID1
    Before_Arena[InstanceID][2] = RoleID2
	Before_Arena[InstanceID][3] = RoleID3
	Before_Arena[InstanceID][4] = RoleID4
	Before_Arena[InstanceID][5] = RoleID5
	Before_Arena[InstanceID][6] = RoleID6

   local ArenaType = role.GetArenaType(RoomID,RoleID1)
	if ArenaType == 0 then
	    local table_Role = {1,2,3,4,5,6}
		local table_Role1 = {}
		for i = 1,6 do
		    local index = math.random(7-i)
		    table_Role1[i] = table_Role[index]
			table.remove(table_Role, index)
		end
        local iRole = {}  --���ID������
		iRole[1] = RoleID1
		iRole[2] = RoleID2
		iRole[3] = RoleID3
		iRole[4] = RoleID4
		iRole[5] = RoleID5
		iRole[6] = RoleID6

		RoleID1 = iRole[table_Role1[1]]
		RoleID2 = iRole[table_Role1[2]]
		RoleID3 = iRole[table_Role1[3]]
		RoleID4 = iRole[table_Role1[4]]
		RoleID5 = iRole[table_Role1[5]]
		RoleID6 = iRole[table_Role1[6]]

    end

    DefineValue_Arena (InstanceID, RoomID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6)
    Arena[InstanceID].Type = ArenaType

	local Arenamember_blue = 0           --������Ӫ������
	local Arenamember_red = 0

	local Role = {}  --���ID������
	Role[1] = RoleID1
	Role[2] = RoleID3
	Role[3] = RoleID5
	Role[4] = RoleID2
	Role[5] = RoleID4
	Role[6] = RoleID6

	local Num = {}     --������������ĵ�ͼ�͸����б�
	Num[1],Num[2] = role.GetRoleMapID(RoleID1)
	Num[3],Num[4] = role.GetRoleMapID(RoleID3)
	Num[5],Num[6] = role.GetRoleMapID(RoleID5)
	Num[7],Num[8] = role.GetRoleMapID(RoleID2)
	Num[9],Num[10] = role.GetRoleMapID(RoleID4)
	Num[11],Num[12] = role.GetRoleMapID(RoleID6)

	local Pos = {}             --���ÿ����ҵ�����
	for i=1,6 do
	    local x,y,z = unit.GetPosition(Num[2*i-1], Num[2*i], Role[i])
		Pos[i] = {Pos_x = x, Pos_y = y, Pos_z = z}
	end

    for i=1,6 do
	    Totle_Arena[InstanceID][i] = Role[i]
	    if Num[2*i-1] == 3017299663 then      --m04
			if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 2024 and Pos[i].Pos_x <= 2124 and Pos[i].Pos_z >= 2615 and Pos[i].Pos_z <= 2715 then
				if JoinArenaLimit(Num[2*i-1], Num[2*i], Role[i]) then
				    if role.IsRoleInStatus(Role[i], 1024) then
						role.CancelRoleStallState(3017299663, -1, Role[i])
					end
					BeInArena[Role[i]] = 1
					MapIDBeforeEnterArena[Role[i]] = 3017299663
					if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]

				--[[else
				    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				    --role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
					role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ]]
				end
			else
			    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end
		elseif Num[2*i-1] == 2092184628 then  --a04
			if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 273 and Pos[i].Pos_x <= 373 and Pos[i].Pos_z >= 277 and Pos[i].Pos_z <= 377 then
				if JoinArenaLimit(Num[2*i-1], Num[2*i], Role[i]) then
				    if role.IsRoleInStatus(Role[i], 1024) then
						role.CancelRoleStallState(2092184628, -1, Role[i])
					end
					BeInArena[Role[i]] = 1
					MapIDBeforeEnterArena[Role[i]] = 2092184628
					if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]
				--[[else
				    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				   -- role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
				    role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ]]
				end
			else
			   -- local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end
	    --elseif Num[2*i-1] == 2092184884 then  --a05
		    --Totle_Arena[InstanceID][i] = Role[i]
			--if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 273 and Pos[i].Pos_x <= 373 and Pos[i].Pos_z >= 277 and Pos[i].Pos_z <= 377 then
					--BeInArena[Role[i]] = 1
					--MapIDBeforeEnterArena[Role[i]] = 2092184884
					--[[if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]
				else
				    local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				    role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
				    role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
				end
			else
			    local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end]]
		elseif Num[2*i-1] == 3017298383 then  --m03
			if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 945 and Pos[i].Pos_x <= 1045 and Pos[i].Pos_z >= 2032 and Pos[i].Pos_z <= 2132 then
				if JoinArenaLimit(Num[2*i-1], Num[2*i], Role[i]) then
				    if role.IsRoleInStatus(Role[i], 1024) then
						role.CancelRoleStallState(3017298383, -1, Role[i])
					end
					BeInArena[Role[i]] = 1
					MapIDBeforeEnterArena[Role[i]] = 3017298383
					if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]
				--[[else
				   -- local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				    --role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
				    role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ]]
				end
			else
			    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end
		elseif Num[2*i-1] == 3017298127 then  --m02
			if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 945 and Pos[i].Pos_x <= 1045 and Pos[i].Pos_z >= 2032 and Pos[i].Pos_z <= 2132 then
				if JoinArenaLimit(Num[2*i-1], Num[2*i], Role[i]) then
				    if role.IsRoleInStatus(Role[i], 1024) then
						role.CancelRoleStallState(3017298127, -1, Role[i])
					end
					BeInArena[Role[i]] = 1
					MapIDBeforeEnterArena[Role[i]] = 3017298127
					if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]
				--[[else
				   -- local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				   -- role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
				    role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ]]
				end
			else
			    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end
		elseif Num[2*i-1] == 3017299919 then  --m05
			if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 945 and Pos[i].Pos_x <= 1045 and Pos[i].Pos_z >= 2032 and Pos[i].Pos_z <= 2132 then
				if JoinArenaLimit(Num[2*i-1], Num[2*i], Role[i]) then
				    if role.IsRoleInStatus(Role[i], 1024) then
						role.CancelRoleStallState(3017299919, -1, Role[i])
					end
					BeInArena[Role[i]] = 1
					MapIDBeforeEnterArena[Role[i]] = 3017299919
					if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]
				--[[else
				    local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				    role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
				    role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ]]
				end
			else
			    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end
		elseif Num[2*i-1] == 3017299151 then  --m06
			if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 945 and Pos[i].Pos_x <= 1045 and Pos[i].Pos_z >= 2032 and Pos[i].Pos_z <= 2132 then
				if JoinArenaLimit(Num[2*i-1], Num[2*i], Role[i]) then
				    if role.IsRoleInStatus(Role[i], 1024) then
						role.CancelRoleStallState(3017299151, -1, Role[i])
					end
					BeInArena[Role[i]] = 1
					MapIDBeforeEnterArena[Role[i]] = 3017299151
					if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]
				--[[else
				   -- local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				   -- role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
				    role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ]]
				end
			else
			    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end
		elseif Num[2*i-1] == 3017299407 then  --m07
			if unit.IsDead(Num[2*i-1], Num[2*i], Role[i]) == false then
				--if Pos[i].Pos_x >= 945 and Pos[i].Pos_x <= 1045 and Pos[i].Pos_z >= 2032 and Pos[i].Pos_z <= 2132 then
				if JoinArenaLimit(Num[2*i-1], Num[2*i], Role[i]) then
				    if role.IsRoleInStatus(Role[i], 1024) then
						role.CancelRoleStallState(3017299407, -1, Role[i])
					end
					BeInArena[Role[i]] = 1
					MapIDBeforeEnterArena[Role[i]] = 3017299407
					if i <= 3 then
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 11000, 50, 2000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 1)
						Arenamember_red = Arenamember_red + 1
					else
						role.RoleGotoArena(Num[2*i-1], Num[2*i], Role[i], MapID, InstanceID, 2000, 50, 11000)
						role.SetForceInArena(MapID, InstanceID, Role[i], 0)
						Arenamember_blue = Arenamember_blue + 1
					end
					Alive_Arena[InstanceID][i] = Role[i]
					PlayerStillInArena[InstanceID][i] = Role[i]
					PlayerTransToArena[InstanceID][i] = Role[i]
				--[[else
				   -- local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				   -- role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
					--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
				    role.DeletRoleFromArenaTable(Role[i])
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
					msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ]]
				end
			else
			   -- local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
				--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			    role.DeletRoleFromArenaTable(Role[i])
				--��ʾ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2502)   --����״̬�µ���Ҳ��ܱ����ͽ�������
				msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
			end
		else
		    --local Point1,Point2 = CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID3, RoleID5, RoleID2, RoleID4, RoleID6, i, 0, 0)
			--role.ModRoleAttValue(MapID, InstanceID, Role[i], 79, Point1)
			--role.ModRoleAttValue(MapID, InstanceID, Role[i], 80, Point2)
			role.DeletRoleFromArenaTable(Role[i])
			--��ʾ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
			msg.DispatchRoleMsgEvent(Role[i], MsgID)     --������Ϣ
		end
	end

	if Arenamember_red == 2 then                   --��������Ӫ����ҽ����������Ĳ���
	    local k = 1
		local MemberID = {}
	    for i=1, 3 do
			if Alive_Arena[InstanceID][i] ~= nil  and  Alive_Arena[InstanceID][i] ~= -1 then
			    MemberID[k] = Alive_Arena[InstanceID][i]
                k = k + 1
			end
		end
		for i = 1, 2 do
		    local Map_ID,Instance_ID = role.GetRoleMapID(MemberID[i])
		    local TeamID = role.IsRoleHaveTeam(Map_ID, Instance_ID, MemberID[i])
			if TeamID ~= nil and TeamID ~= 4294967295 then
		        role.LetRoleLeaveTeam(TeamID, MemberID[i])
			end
		end
		role.CreatANewTeam(MemberID[1], MemberID[2])
	elseif Arenamember_red == 3 then
	    local k = 1
		local MemberID = {}
	    for i=1, 3 do
			if Alive_Arena[InstanceID][i] ~= nil  and  Alive_Arena[InstanceID][i] ~= -1 then
			    MemberID[k] = Alive_Arena[InstanceID][i]
                k = k + 1
			end
		end
		for i = 1, 3 do
		    local Map_ID,Instance_ID = role.GetRoleMapID(MemberID[i])
		    local TeamID = role.IsRoleHaveTeam(Map_ID, Instance_ID, MemberID[i])
			if TeamID ~= nil and TeamID ~= 4294967295 then
		        role.LetRoleLeaveTeam(TeamID, MemberID[i])
			end
		end
		local TeamID = role.CreatANewTeam(MemberID[1], MemberID[2])
		role.LetRoleJoinTeam(TeamID, MemberID[3])
	elseif Arenamember_red == 1 then
		local k = 1
		local MemberID = 0
	    for i=1, 3 do
			if Alive_Arena[InstanceID][i] ~= nil  and  Alive_Arena[InstanceID][i] ~= -1 then
			    MemberID = Alive_Arena[InstanceID][i]
                break
			end
		end
		if MemberID ~= 0 then
			local Map_ID,Instance_ID = role.GetRoleMapID(MemberID)
		    local TeamID = role.IsRoleHaveTeam(Map_ID, Instance_ID, MemberID)
			if TeamID ~= nil and TeamID ~= 4294967295 then
		        role.LetRoleLeaveTeam(TeamID, MemberID)
			end
		end
	end

	if Arenamember_blue == 2 then                   --��������Ӫ����ҽ����������Ĳ���
	    local k = 1
		local MemberID = {}
	    for i=4, 6 do
			if Alive_Arena[InstanceID][i] ~= nil  and  Alive_Arena[InstanceID][i] ~= -1 then
			    MemberID[k] = Alive_Arena[InstanceID][i]
                k = k + 1
			end
		end
		for i = 1, 2 do
		    local Map_ID,Instance_ID = role.GetRoleMapID(MemberID[i])
		    local TeamID = role.IsRoleHaveTeam(Map_ID, Instance_ID, MemberID[i])
			if TeamID ~= nil and TeamID ~= 4294967295 then
		        role.LetRoleLeaveTeam(TeamID, MemberID[i])
			end
		end
		role.CreatANewTeam(MemberID[1], MemberID[2])
	elseif Arenamember_blue ==3 then
	    local k = 1
		local MemberID = {}
	    for i=4, 6 do
			if Alive_Arena[InstanceID][i] ~= nil  and  Alive_Arena[InstanceID][i] ~= -1 then
			    MemberID[k] = Alive_Arena[InstanceID][i]
                k = k + 1
			end
		end
		for i = 1, 3 do
		    local Map_ID,Instance_ID = role.GetRoleMapID(MemberID[i])
		    local TeamID = role.IsRoleHaveTeam(Map_ID, Instance_ID, MemberID[i])
			if TeamID ~= nil and TeamID ~= 4294967295 then
		        role.LetRoleLeaveTeam(TeamID, MemberID[i])
			end
		end
		local TeamID = role.CreatANewTeam(MemberID[1], MemberID[2])
		role.LetRoleJoinTeam(TeamID, MemberID[3])
	elseif Arenamember_blue == 1 then
		local MemberID = 0
	    for i=4, 6 do
			if Alive_Arena[InstanceID][i] ~= nil  and  Alive_Arena[InstanceID][i] ~= -1 then
			    MemberID = Alive_Arena[InstanceID][i]
                break
			end
		end
		if MemberID~= 0 then
			local Map_ID,Instance_ID = role.GetRoleMapID(MemberID)
		    local TeamID = role.IsRoleHaveTeam(Map_ID, Instance_ID, MemberID)
			if TeamID ~= nil and TeamID ~= 4294967295 then
		        role.LetRoleLeaveTeam(TeamID, MemberID)
			end
		end
	end

	Arena[InstanceID].red = Arenamember_red          --��¼�����л����ŵ�˫��������
	Arena[InstanceID].blue = Arenamember_blue

	--[[for i= 1, 6 do              --�����뾺��������Ҽ�������BUFF
	    if Alive_Arena[InstanceID][i] ~= -1 then
		    unit.AddBuff(MapID, InstanceID, Alive_Arena[InstanceID][i], 2025101, Alive_Arena[InstanceID][i])
			unit.AddBuff(MapID, InstanceID, Alive_Arena[InstanceID][i], 2025201, Alive_Arena[InstanceID][i])
		end
	end]]

    for i=1, 6 do           --�����������ʾ�������
		if PlayerTransToArena[InstanceID][i] ~= -1 then
			if i<= 3 then
				role.ShowAlivePeopleInArena(PlayerTransToArena[InstanceID][i], Arena[InstanceID].blue, Arena[InstanceID].red)
			else
				role.ShowAlivePeopleInArena(PlayerTransToArena[InstanceID][i], Arena[InstanceID].red, Arena[InstanceID].blue)
			end
		end
	end

	local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900056, 6, 1, 9)
	local creature2 = map.MapCreateCreature(MapID, InstanceID, 4900055, 6, 1, 9)
	local creature3 = map.MapCreateCreature(MapID, InstanceID, 4900057, 6, 1, 9)
	local creature4 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)

	ArenaCreature_Timer[creature1]= {TypeID = 1}      --1���ӿ���
	ArenaCreature_Timer[creature2]= {TypeID = 3}      --30���ʱ
	ArenaCreature_Timer[creature3]= {TypeID = 2}      --10���Ӽ�ʱ
	ArenaCreature_Timer[creature4]= {TypeID = 9}      --15��

end



--��ҽ��뾺������ʱ�����BeInArena��ֵΪ0����Ϊ�գ����䴫�����������䳡
function a06_OnPlayerEnter(MapID, InstanceID, RoleID)
    unit.AddBuff(MapID, InstanceID, RoleID, 2025101, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 2025201, RoleID)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel ~= nil then
	    if rolelevel >= 100 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025708, RoleID)
		elseif rolelevel >= 90 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025707, RoleID)
		elseif rolelevel >= 80 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025706, RoleID)
		elseif rolelevel >= 70 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025705, RoleID)
		elseif rolelevel >= 60 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025704, RoleID)
		elseif rolelevel >= 50 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025703, RoleID)
		elseif rolelevel >= 40 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025702, RoleID)
		elseif rolelevel >= 30 then
		    unit.AddBuff(MapID, InstanceID, RoleID, 2025701, RoleID)
		end
	end
    if BeInArena[RoleID] == nil or BeInArena[RoleID] ~= 1 then
	    if MapIDBeforeEnterArena[RoleID] ~= nil and MapIDBeforeEnterArena[RoleID] == 3017299663 then
		    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017299663, 2071, 367, 2666)
		elseif MapIDBeforeEnterArena[RoleID] ~= nil and MapIDBeforeEnterArena[RoleID] == 2092184884 then
		    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2092184628, 333, 43, 308)
		elseif MapIDBeforeEnterArena[RoleID] ~= nil and MapIDBeforeEnterArena[RoleID] == 3017298383 then
		    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298383, 995, 200, 2082)
		elseif MapIDBeforeEnterArena[RoleID] ~= nil and MapIDBeforeEnterArena[RoleID] == 3017298127 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298127, 2087, 6646, 2286)
		elseif MapIDBeforeEnterArena[RoleID] ~= nil and MapIDBeforeEnterArena[RoleID] == 3017299919 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017299919, 1050, 20140, 957)
		elseif MapIDBeforeEnterArena[RoleID] ~= nil and MapIDBeforeEnterArena[RoleID] == 3017299151 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017299151, 2121, 6365, 1815)
		elseif MapIDBeforeEnterArena[RoleID] ~= nil and MapIDBeforeEnterArena[RoleID] == 3017299407 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017299407, 1875, 14232, 2468)
		else
	        role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2092184628, 333, 43, 308)
		end
	end
end



--����ھ�������ɱ��
function JinngJiChang_KillPeople(MapID, InstanceID, RoleID, KillerID)
    local Num1 = SearchARoleID(Totle_Arena[InstanceID], RoleID)
	if Num1 ~= -1 then
	    if unit.IsPlayer(KillerID) then
			local Num2 = SearchARoleID(Totle_Arena[InstanceID], KillerID)
			if Num2 ~= -1 then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2503)           --���Ѿ�������ɱ��������<color=0xffffff00>XXX<color=0xfffff7e0>��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

				KillPeople_Arena[InstanceID][Num2] = KillPeople_Arena[InstanceID][Num2] + 1
				role.ShowKillerPictureInArena(Totle_Arena[InstanceID][Num2], KillPeople_Arena[InstanceID][Num2])
				for i =1,6 do
					if PlayerStillInArena[InstanceID][i] ~= -1 and  i ~= Num2 and PlayerStillInArena[InstanceID][i] ~= RoleID then
						if Num2 <= 3 then
							if i <= 3 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 2504)          --<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����<color=0xffff0000>XXX<color=0xfffff7e0>
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 2505)          --���Ķ���<color=0xffff0000>XXX<color=0xfffff7e0>��<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
							end
						else
							if i <= 3 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 2505)          --���Ķ���<color=0xffff0000>XXX<color=0xfffff7e0>��<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 2504)          --<color=0xffffff00>XXX<color=0xfffff7e0>ɱ����<color=0xffff0000>XXX<color=0xfffff7e0>
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
							end
						end
					end
				end
			end
		end
		Alive_Arena[InstanceID][Num1] = -1
		if Num1 <= 3 then
		    if unit.IsPlayer(KillerID) and KillerID ~= RoleID then
		        local IsInArea = map.IsInArea(MapID, InstanceID, RoleID, 245)
				if IsInArea == 1 then
				    local ArenaPoint = RankPoint_Arena[InstanceID][Num1].ArenaPoint
					local RankArenaPoint_Max = ArenaPoint_Max(RankPoint_Arena[InstanceID][Num1].RankPoint)
					if ArenaPoint ~= nil and RankArenaPoint_Max ~= nil and ArenaPoint < RankArenaPoint_Max then
				        role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][Num1], 80, 1)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2530)       --��ɱ����һ�����ڵз������еĵ��ˣ���ö����һ���似ֵ
						msg.DispatchRoleMsgEvent(KillerID, MsgID)     --������Ϣ
					end
				end
			end

			Arena[InstanceID].red = Arena[InstanceID].red - 1
			for i=1, 6 do           --�����������ʾ�������
				if PlayerStillInArena[InstanceID][i] ~= -1 then
					if i<= 3 then
						role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].blue, Arena[InstanceID].red)
					else
						role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].red, Arena[InstanceID].blue)
					end
				end
			end
			if Arena[InstanceID].red == 0 then
			    if BattleEnd_Arena[InstanceID] ~= 1 then

					BattleEnd_Arena[InstanceID] = 1
					ShowFinalFrame(MapID, InstanceID)

					local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
					ArenaCreature_Timer[creature1]= {TypeID = 6}      --15�봫�ͼ�ʱ
				end
			end
		else
		    if unit.IsPlayer(KillerID) and KillerID ~= RoleID then
		        local IsInArea = map.IsInArea(MapID, InstanceID, RoleID, 246)
				if IsInArea == 1 then
				    local ArenaPoint = RankPoint_Arena[InstanceID][Num1].ArenaPoint
					local RankArenaPoint_Max = ArenaPoint_Max(RankPoint_Arena[InstanceID][Num1].RankPoint)
					if ArenaPoint ~= nil and RankArenaPoint_Max ~= nil and ArenaPoint < RankArenaPoint_Max then
				        role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][Num1], 80, 1)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2530)       --��ɱ����һ�����ڵз������еĵ��ˣ���ö����һ���似ֵ
						msg.DispatchRoleMsgEvent(KillerID, MsgID)     --������Ϣ
					end
				end
			end
			Arena[InstanceID].blue = Arena[InstanceID].blue - 1
			for i=1, 6 do           --�����������ʾ�������
				if PlayerStillInArena[InstanceID][i] ~= -1 then
					if i<= 3 then
						role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].blue, Arena[InstanceID].red)
					else
						role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].red, Arena[InstanceID].blue)
					end
				end
			end
			if Arena[InstanceID].blue == 0 then
				if BattleEnd_Arena[InstanceID] ~= 1 then

					BattleEnd_Arena[InstanceID] = 1
					ShowFinalFrame(MapID, InstanceID)

					local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
					ArenaCreature_Timer[creature1]= {TypeID = 6}      --15�봫�ͼ�ʱ
				end
			end
		end

	end
end




--����뿪������

function a06_LeaveMap(MapID, InstanceID, RoleID)
	if Totle_Arena[InstanceID] ~= nil and BeInArena[RoleID] ~= nil then
		unit.UnsetState(MapID, InstanceID, RoleID, 10)
		unit.UnsetState(MapID, InstanceID, RoleID, 11)
		local Num1 = SearchARoleID(Totle_Arena[InstanceID], RoleID)
		BeInArena[RoleID] = nil
		if Num1 ~= -1 and PlayerStillInArena[InstanceID][Num1] ~= -1 then
		    if BattleEnd_Arena[InstanceID] ~= 1 then
				local TotleTimes_Arena = role.GetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"])
				role.SetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)
				for i=1,6 do
					if i~= Num1 then
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2506)   --XXXǿ���뿪�˾�����
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.DispatchRoleMsgEvent(Totle_Arena[InstanceID][i], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2506)   --XXXǿ���뿪�˾�����
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.DispatchRoleMsgEvent(Totle_Arena[InstanceID][i], MsgID)     --������Ϣ
					end
				end

				local Point1,Point2 = CountPoint_Arena(Arena[InstanceID].Room, InstanceID, Totle_Arena[InstanceID][1], Totle_Arena[InstanceID][2], Totle_Arena[InstanceID][3], Totle_Arena[InstanceID][4], Totle_Arena[InstanceID][5], Totle_Arena[InstanceID][6], Num1, 0, 0)
				role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][Num1], 79, Point1)
				role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][Num1], 80, Point2)
				role.DeletRoleFromArenaTable(Totle_Arena[InstanceID][Num1])
				if Alive_Arena[InstanceID][Num1] ~= -1 then
					if Num1 <= 3 then
						Arena[InstanceID].red = Arena[InstanceID].red - 1
						for i=1, 6 do           --�����������ʾ�������
							if PlayerStillInArena[InstanceID][i] ~= -1 then
								if i<= 3 then
									role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].blue, Arena[InstanceID].red)
								else
									role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].red, Arena[InstanceID].blue)
								end
							end
						end
						if Arena[InstanceID].red == 0 then
							if BattleEnd_Arena[InstanceID] ~= 1 then

								BattleEnd_Arena[InstanceID] = 1
								ShowFinalFrame(MapID, InstanceID)

								local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
								ArenaCreature_Timer[creature1]= {TypeID = 6}      --15�봫�ͼ�ʱ
							end
						end
					else
						Arena[InstanceID].blue = Arena[InstanceID].blue - 1
						for i=1, 6 do           --�����������ʾ�������
							if PlayerStillInArena[InstanceID][i] ~= -1 then
								if i<= 3 then
									role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].blue, Arena[InstanceID].red)
								else
									role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].red, Arena[InstanceID].blue)
								end
							end
						end
						if Arena[InstanceID].blue == 0 then
							if BattleEnd_Arena[InstanceID] ~= 1 then

								BattleEnd_Arena[InstanceID] = 1
								ShowFinalFrame(MapID, InstanceID)

								local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
								ArenaCreature_Timer[creature1]= {TypeID = 6}      --15�봫�ͼ�ʱ
							end
						end
					end
				end
			else
				role.DeletRoleFromArenaTable(Totle_Arena[InstanceID][Num1])
			end
		end
		PlayerStillInArena[InstanceID][Num1] = -1
		Alive_Arena[InstanceID][Num1] = -1
	end
end



--��Ҹ���ű�
function JinngJiChang_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

    ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
	ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
    role.DeletRoleFromArenaTable(RoleID)
	if MapIDBeforeEnterArena[RoleID] == 3017299663 then
	    return ReviveHP, ReviveMP, 2071, 18345, 2666, 3017299663
	elseif MapIDBeforeEnterArena[RoleID] == 2092184628 then
	    return ReviveHP, ReviveMP, 331, 2150, 307, 2092184628
	elseif MapIDBeforeEnterArena[RoleID] == 3017298383 then
	    return ReviveHP, ReviveMP, 995, 10000, 2082, 3017298383
	elseif MapIDBeforeEnterArena[RoleID] == 3017298127 then
		return ReviveHP, ReviveMP, 2087, 6646, 2286, 3017298127
	elseif MapIDBeforeEnterArena[RoleID] == 3017299919 then
		return ReviveHP, ReviveMP, 1050, 22140, 957, 3017299919
	elseif MapIDBeforeEnterArena[RoleID] == 3017299151 then
		return ReviveHP, ReviveMP, 2121, 7365, 1815, 3017299151
	elseif MapIDBeforeEnterArena[RoleID] == 3017299407 then
		return ReviveHP, ReviveMP, 1875, 15232, 2468, 3017299407
	else
	    return ReviveHP, ReviveMP, 331, 2150, 307, 2092184628
	end

end



--��15��������ʱ�䣬����ʱ��ʹ��
function c4900054_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 4 then
	    if BattleEnd_Arena[InstanceID] ~= nil and BattleEnd_Arena[InstanceID] ~= 1 then
			for i=1,6 do
				if PlayerStillInArena[InstanceID][i] ~= nil and PlayerStillInArena[InstanceID][i] ~= -1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2507)          --ս������<color=0xffffff00>15<color=0xfffff7e0>���ʼ
					msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
				end
			end
		end
	elseif ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 9 then
        for i=1,6 do
			if PlayerStillInArena[InstanceID][i] ~= nil and PlayerStillInArena[InstanceID][i] ~= -1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2514)          --���Ž���<color=0xffffff00>45<color=0xfffff7e0>����
				msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
			end
		end
	elseif ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 10 then
        RemoveValue_Arena (InstanceID)
	elseif ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 6 then
	    for i=1,6 do
		    unit.UnsetState(MapID, InstanceID, Totle_Arena[InstanceID][i], 10)
		    unit.UnsetState(MapID, InstanceID, Totle_Arena[InstanceID][i], 11)
		    if PlayerStillInArena[InstanceID][i] ~= nil and PlayerStillInArena[InstanceID][i] ~= -1 then
			    if unit.IsDead(MapID, InstanceID, PlayerStillInArena[InstanceID][i]) then
				    role.ForcedToRevive(MapID, InstanceID, PlayerStillInArena[InstanceID][i])
				else
				    if MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] == 3017299663 then
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 3017299663, 2071, 367, 2666)
					elseif MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] == 2092184628 then
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 2092184628, 331, 43, 307)
					elseif MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] == 3017298383 then
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 3017298383, 995, 200, 2082)
					elseif MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] == 3017298127 then
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 3017298127, 2087, 6646, 2286)
					elseif MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] == 3017299919 then
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 3017299919, 1050, 20140, 957)
					elseif MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] == 3017299151 then
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 3017299151, 2121, 6365, 1815)
					elseif MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] == 3017299407 then
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 3017299407, 1875, 14232, 2468)
					else
						role.RoleGotoNewMap(MapID, InstanceID, PlayerStillInArena[InstanceID][i], 2092184628, 331, 43, 307)
					end
				end
			else
			    PlayerStillInArena[InstanceID][i] = 0
			end
			if PlayerStillInArena[InstanceID][i] == nil or PlayerStillInArena[InstanceID][i] == -1 or PlayerStillInArena[InstanceID][i] == 0 then
			    for k = 1,6 do
				    if Before_Arena[InstanceID][k] == Totle_Arena[InstanceID][i] then
					    Before_Arena[InstanceID][k] = 0
					    break
					end
				end
			end
		end
        if Arena[InstanceID].Type == 0 and act.GetActIsStart(147) ~= true then
		    for k=1,6 do
			    if Before_Arena[InstanceID][k] ~= nil and Before_Arena[InstanceID][k] ~= 0 then
					role.DeletRoleFromArenaTable(Before_Arena[InstanceID][k])
				end
			end
			role.ShowArenaRoomFrame(InstanceID, 0, 0, 0, 0, 0, 0)
		else
		    role.ShowArenaRoomFrame(InstanceID, Before_Arena[InstanceID][1], Before_Arena[InstanceID][2], Before_Arena[InstanceID][3], Before_Arena[InstanceID][4], Before_Arena[InstanceID][5], Before_Arena[InstanceID][6])
		end
		local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
	    ArenaCreature_Timer[creature1]= {TypeID = 10}      --15���ͷ��ڴ�
		--RemoveValue_Arena (InstanceID)
	end
	ArenaCreature_Timer[TargetID] = nil
end



--��30��������ʱ�䣬����ʱ��ʹ��
function c4900055_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 3 then
	    if BattleEnd_Arena[InstanceID] ~= nil and BattleEnd_Arena[InstanceID] ~= 1 then
			if Arena[InstanceID].red == 0 then

				BattleEnd_Arena[InstanceID] = 1
				ShowFinalFrame(MapID, InstanceID)

				local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
				ArenaCreature_Timer[creature1]= {TypeID = 6}      --15�봫�ͼ�ʱ

			elseif Arena[InstanceID].blue == 0 then

				BattleEnd_Arena[InstanceID] = 1
				ShowFinalFrame(MapID, InstanceID)

				local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
				ArenaCreature_Timer[creature1]= {TypeID = 6}      --15�봫�ͼ�ʱ

			else
				local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
				ArenaCreature_Timer[creature1]= {TypeID = 4}      --15��
			end
	    end
	elseif ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 7 then
	    if BattleEnd_Arena[InstanceID] ~= nil and BattleEnd_Arena[InstanceID] ~= 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 2508)          --ս������<color=0xffffff00>30<color=0xfffff7e0>������
			for i=1,6 do
				if PlayerStillInArena[InstanceID][i] ~= -1 and  PlayerStillInArena[InstanceID][i] ~= nil then
					msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
				end
			end
        end
	elseif ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 8 then
        if BattleEnd_Arena[InstanceID] ~= nil and BattleEnd_Arena[InstanceID] ~= 1 then
			if BattleEnd_Arena[InstanceID] ~=nil and BattleEnd_Arena[InstanceID] ~= 1 then
				local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900055, 6, 1, 9)
				ArenaCreature_Timer[creature1]= {TypeID = 8}      --30���Ӽ�ʱ
				Arena_Minutes[InstanceID] = Arena_Minutes[InstanceID] + 1
			end
		end
	end
    ArenaCreature_Timer[TargetID] = nil
end



--��1���ӹ������ʱ�䣬����ʱ��ʹ��
function c4900056_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)

   -- if ArenaCreature_Timer[TargetID].TypeID ~= nil and ArenaCreature_Timer[TargetID].TypeID == 1 then
        local ArenaType = 1
		if PlayerStillInArena[InstanceID] ~= nil then
			for i=1,6 do
				if PlayerStillInArena[InstanceID][i] ~= nil and PlayerStillInArena[InstanceID][i] ~= -1 then
					ArenaType = role.GetArenaType(Arena[InstanceID].Room, PlayerStillInArena[InstanceID][i])
					break
				end
			end
		end

        if BattleEnd_Arena[InstanceID] ~= nil and BattleEnd_Arena[InstanceID] ~= 1 then
            map.OpenCloseDoor(MapID, InstanceID, 225, 1)
			map.OpenCloseDoor(MapID, InstanceID, 226, 1)
			BattleBegin_Arena[InstanceID] = 1
			Arena_Minutes[InstanceID] = Arena_Minutes[InstanceID] + 2

			local MsgID = msg.BeginMsgEvent()
			if ArenaType == 0 then
				msg.AddMsgEvent(MsgID, 101, 2541)          --<color=0xffff0000>���ģʽս����ʼ<color=0xfffff7e0>
			else
				msg.AddMsgEvent(MsgID, 101, 2509)          --<color=0xffff0000>ս����ʼ<color=0xfffff7e0>
			end
			for i=1,6 do
				if PlayerStillInArena[InstanceID][i] ~= nil and PlayerStillInArena[InstanceID][i] ~= -1 then
					msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
				end
			end
			local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900057, 6, 1, 9)
			ArenaCreature_Timer[creature1]= {TypeID = 5}      --10���ӽ����ʱ

			local creature2 = map.MapCreateCreature(MapID, InstanceID, 4900055, 6, 1, 9)
			ArenaCreature_Timer[creature2]= {TypeID = 8}      --30���Ӽ�ʱ
        end
	--end

	ArenaCreature_Timer[TargetID] = nil

end



--��10���ӵĹ������ʱ�䣬����ʱ��ʹ��
function c4900057_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)

    if ArenaCreature_Timer[TargetID]~= nil and ArenaCreature_Timer[TargetID].TypeID == 2 then
	    if BattleEnd_Arena[InstanceID] ~= nil and BattleEnd_Arena[InstanceID] ~= 1 then
			for i=1,6 do
				if PlayerStillInArena[InstanceID][i] ~= nil and PlayerStillInArena[InstanceID][i] ~= -1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2510)          --ս������<color=0xffffff00>1<color=0xfffff7e0>���Ӻ����
					msg.DispatchRoleMsgEvent(PlayerStillInArena[InstanceID][i], MsgID)     --������Ϣ
				end
			end
			local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900055, 6, 1, 9)
			ArenaCreature_Timer[creature1]= {TypeID = 7}      --30������ʱ
		end
	else
	    if BattleEnd_Arena[InstanceID] ~= nil and BattleEnd_Arena[InstanceID] ~= 1 then
		    BattleEnd_Arena[InstanceID] = 1

		    ShowFinalFrame(MapID, InstanceID)

		    local creature1 = map.MapCreateCreature(MapID, InstanceID, 4900054, 6, 1, 9)
	        ArenaCreature_Timer[creature1]= {TypeID = 6}      --15�봫�ͼ�ʱ
		end
	end

	ArenaCreature_Timer[TargetID] = nil
end


--�������Ի������Ի������BUFF�е�һ��
function n4006513_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local Num = SearchARoleID(Totle_Arena[InstanceID], RoleID)
	if Num == -1 then
	    Num = 0
	end
	local a = map.GetMapScriptData(MapID,InstanceID,1,Num)
    if TalkIndex == -1 then

		if a ~= 0 and BattleBegin_Arena[InstanceID] ~= nil and BattleBegin_Arena[InstanceID]== 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2515) -- "����飺\n    ������ʼ��ǰ������Դ�������������״̬��ѡ��һ����ӵ��Լ����ϡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
            local buff1 = a % 100
			local buff2 = (a % 10000 - buff1)/ 100
			local buff3 = (a - buff2 * 100 - buff1)/ 10000
			msg.AddMsgEvent(MsgID, 21, 10 + buff1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 2515 + buff1) -- "XXX������XX���ԣ�"
			msg.AddMsgEvent(MsgID, 21, 10 + buff2) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 2515 + buff2) -- "XXX������XX���ԣ�"
			msg.AddMsgEvent(MsgID, 21, 10 + buff3) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 2515 + buff3) -- "XXX������XX���ԣ�"
			msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 2004)   --���´���˵�ɡ�
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif BattleBegin_Arena[InstanceID] ~= nil and BattleBegin_Arena[InstanceID]== 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2526) -- "����飺\n    �Բ���ս���Ѿ���ʼ�����޷��ٴ�������ȡ�κ�״̬��"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 21)   --��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2527) -- "����飺\n    ����ս�����Ѿ���ȡ������״̬�ˡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 21)   --��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end

    elseif TalkIndex >= 11 and TalkIndex <= 20 then
	    unit.AddBuff(MapID, InstanceID, RoleID, 2025701 + (TalkIndex - 10)*100, RoleID)
	    map.SetMapScriptData(MapID,InstanceID,1,Num,0)
    end
end

--��ҽ�������
function Arena_OnRoleEnterArea(MapID, InstanceID, RoleID, ObjAreaID)

    local Num = SearchARoleID(Totle_Arena[InstanceID], RoleID)
	if ObjAreaID == 246 then
	    if Num ~= -1 then
		    if Num <= 3 then
			    local a = map.GetMapScriptData(MapID,InstanceID,1,Num+6)
				if a ~= 1 then
				    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				    if rolelevel ~= nil then
						if rolelevel >= 100 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027501, RoleID)
						elseif rolelevel >= 90 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027401, RoleID)
						elseif rolelevel >= 80 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027301, RoleID)
						elseif rolelevel >= 70 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027201, RoleID)
						elseif rolelevel >= 60 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027101, RoleID)
						elseif rolelevel >= 50 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027001, RoleID)
						elseif rolelevel >= 40 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2026901, RoleID)
						elseif rolelevel >= 30 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2026801, RoleID)
						end
					end
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2528) -- "���뼺���������򣬻�����ݴ���״̬"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					map.SetMapScriptData(MapID,InstanceID,1,Num+6,1)
				end
			end
		end
	elseif ObjAreaID == 245 then
	    if Num ~= -1 then
		    if Num <= 6 and Num >= 4 then
			    local a = map.GetMapScriptData(MapID,InstanceID,1,Num+6)
				if a ~= 1 then
				    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				    if rolelevel ~= nil then
						if rolelevel >= 100 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027501, RoleID)
						elseif rolelevel >= 90 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027401, RoleID)
						elseif rolelevel >= 80 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027301, RoleID)
						elseif rolelevel >= 70 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027201, RoleID)
						elseif rolelevel >= 60 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027101, RoleID)
						elseif rolelevel >= 50 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2027001, RoleID)
						elseif rolelevel >= 40 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2026901, RoleID)
						elseif rolelevel >= 30 then
							unit.AddBuff(MapID, InstanceID, RoleID, 2026801, RoleID)
						end
					end
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2528) -- "���뼺���������򣬻�����ݴ���״̬"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					map.SetMapScriptData(MapID,InstanceID,1,Num+6,1)
				end
			end
		end
	end
end


--����뿪����
function Arena_OnRoleLeaveArea(MapID, InstanceID, RoleID, ObjAreaID)

    local Num = SearchARoleID(Totle_Arena[InstanceID], RoleID)
	if ObjAreaID == 246 then
	    if Num ~= -1 then
		    if Num <= 3 then
			    local a = map.GetMapScriptData(MapID,InstanceID,1,Num+6)
				if a ~= 0 then
				    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				    if rolelevel ~= nil then
						if rolelevel >= 100 then
						    unit.CancelBuff(MapID, InstanceID, RoleID, 2027501)
						elseif rolelevel >= 90 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027401)
						elseif rolelevel >= 80 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027301)
						elseif rolelevel >= 70 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027201)
						elseif rolelevel >= 60 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027101)
						elseif rolelevel >= 50 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027001)
						elseif rolelevel >= 40 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2026901)
						elseif rolelevel >= 30 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2026801)
						end
					end
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2529) -- "�뿪���������������ݴ���״̬��ʧ"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					map.SetMapScriptData(MapID,InstanceID,1,Num+6,0)
				end
			end
		end
	elseif ObjAreaID == 245 then
	    if Num ~= -1 then
		    if Num <= 6 and Num >= 4 then
			    local a = map.GetMapScriptData(MapID,InstanceID,1,Num+6)
				if a ~= 0 then
				    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				    if rolelevel ~= nil then
						if rolelevel >= 100 then
						    unit.CancelBuff(MapID, InstanceID, RoleID, 2027501)
						elseif rolelevel >= 90 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027401)
						elseif rolelevel >= 80 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027301)
						elseif rolelevel >= 70 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027201)
						elseif rolelevel >= 60 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027101)
						elseif rolelevel >= 50 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2027001)
						elseif rolelevel >= 40 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2026901)
						elseif rolelevel >= 30 then
							unit.CancelBuff(MapID, InstanceID, RoleID, 2026801)
						end
					end
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2529) -- "�뿪���������������ݴ���״̬��ʧ"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					map.SetMapScriptData(MapID,InstanceID,1,Num+6,0)
				end
			end
		end
	end
end

--���������뷿��
function JoinArenaRoom(MapID, InstanceID, Type, RoomID, RoleID)
	if Type == 1 then
		if act.GetActIsStart(147) ~= true then
			local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 26, 2542)   --������������δ��ʼ���޷�ִ�иò���
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			return false
		elseif JoinArenaRoomLimit(MapID, InstanceID, RoleID) then
		    return 1
		else
		    return false
		end
	else
	    if JoinArenaRoomLimit(MapID, InstanceID, RoleID) then
		    return 1
		else
		    return false
		end
	end
end

--��ҽ�������������뷿��
function AcceptJoinArenaRoom(MapID, InstanceID, Type, RoomID, HoseID, RoleID)
    local wuxun, lvmin,lvmax = role.GetArenaRoomLimit(Type, RoomID)
	if lvmin == nil then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2548)   --Ŀ�귿���Ѿ��رգ��޷����뷿��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	    return false
	end
	local RankPoint = role.GetRoleAttValue(MapID, InstanceID, RoleID, 79)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    RankPoint = RankPoint_Self(RankPoint)
	if RankPoint < wuxun or lvmin > rolelevel and lvmax < rolelevel then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2546)   --����������뾺���������Ҫ���޷���������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2547)   --XXX��������뾺���������Ҫ���޷���������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(HoseID, MsgID)     --������Ϣ
		return false
	end

    if role.IsRoleInStatus(RoleID, 4) and MapID ~= 2092184628 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2539)   --������������״̬���޷����뾺��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2543)   --XXX����������״̬���޷����뾺��������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(HoseID, MsgID)     --������Ϣ
		return false
	end

	if role.IsRoleInStatus(RoleID, 65536) then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2540)   --������������״̬���޷����뾺��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        --role.DeletRoleFromArenaTable(RoleID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2544)   --XXX����������״̬���޷����뾺��������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(HoseID, MsgID)     --������Ϣ
		return false
	end

    if Type == 1 then
		if act.GetActIsStart(147) ~= true then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2542)   --������������δ��ʼ���޷�ִ�иò���
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.DispatchRoleMsgEvent(HoseID, MsgID)     --������Ϣ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2542)   --������������δ��ʼ���޷�ִ�иò���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			return false
		end
	end

	local x, y, z= unit.GetPosition(MapID, InstanceID, RoleID)
	if MapID == 3017298127 and x < 2400 and x > 2000 and z <2500 and z > 2060 then
		return 1
	elseif MapID == 3017298383 and x < 1150 and x > 629 and z <2400 and z > 1855 then
		return 1
	elseif MapID == 3017299663 and x < 2250 and x > 1940 and z <2990 and z > 2280 then
		return 1
	elseif MapID == 3017299919 and x < 1600 and x > 690 and z <1240 and z > 600 then
		return 1
	elseif MapID == 3017299151 and x < 2300 and x > 1980 and z <1900 and z > 1640 then
		return 1
	elseif MapID == 3017299407 and x < 2040 and x > 1650 and z <3000 and z > 2170 then
		return 1
	elseif MapID == 2092184628 then
	    return 1
	else
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2501)   --ֻ�д��������л������䳡����Ҳ��ܽ��뾺��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2545)   --XXX����λ���޷����뾺��������
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(HoseID, MsgID)     --������Ϣ
		return false
	end
end

--ע��
aux.RegisterMapEvent("a06", 24, "a06_ArenaOnStart")
aux.RegisterMapEvent("a06", 2, "a06_OnPlayerEnter")
aux.RegisterMapEvent("a06", 5, "JinngJiChang_KillPeople")
aux.RegisterMapEvent("a06", 3, "a06_LeaveMap")
aux.RegisterMapEvent("a06", 11, "JinngJiChang_RoloRevive")
aux.RegisterCreatureEvent(4900054, 13, "c4900054_OnDisappear")
aux.RegisterCreatureEvent(4900055, 13, "c4900055_OnDisappear")
aux.RegisterCreatureEvent(4900056, 13, "c4900056_OnDisappear")
aux.RegisterCreatureEvent(4900057, 13, "c4900057_OnDisappear")
aux.RegisterCreatureEvent(4006513, 7, "n4006513_OnTalk")
aux.RegisterMapEvent("a06", 7, "Arena_OnRoleEnterArea")
aux.RegisterMapEvent("a06", 23, "Arena_OnRoleLeaveArea")
aux.RegisterRoleEvent(36, "JoinArenaRoom")
aux.RegisterRoleEvent(37, "JoinArenaRoom")
aux.RegisterRoleEvent(38, "AcceptJoinArenaRoom")

