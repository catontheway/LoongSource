
function moshenchuansong(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Jidou_Times = role.GetRoleScriptData(RoleID, 1, RoleDataType["Jidou_Times"])
	local Jidou_Last = role.GetRoleScriptData(RoleID, 1, RoleDataType["Jidou_Last"])

	local curdate = tonumber(os.date("%j"))

	if curdate ~= Jidou_Last then
		Jidou_Last = curdate
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Jidou_Last"], Jidou_Last)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Jidou_Times"], 0)
	end
-----------------------------------------------�ֲ���������------------------------------------------------
	local function MSJD_JoinTeam(MapID, InstanceID, tbl)        --tbl������һ��8��Ԫ�ص����ID��
	    for k,v in pairs(tbl) do
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, v)
			if TeamID ~= nil and TeamID ~= 4294967295 then
				role.LetRoleLeaveTeam(TeamID, v)
			end
		end
        local TeamID1 = role.CreatANewTeam(tbl[1], tbl[2])
		role.LetRoleJoinTeam(TeamID1, tbl[3])
		role.LetRoleJoinTeam(TeamID1, tbl[4])
		local TeamID2 = role.CreatANewTeam(tbl[5], tbl[6])
		role.LetRoleJoinTeam(TeamID2, tbl[7])
		role.LetRoleJoinTeam(TeamID2, tbl[8])
	end

-----------------------------------------------Ĭ��ֵ-------------------------------------------------
	local function MSJD_VALUE(InctancID,tbl)        --tbl������һ��8��Ԫ�ص����ID��
	    Jidou.RoomInfo[InctancID] = {IsOver = 0,BeginTime = os.time(),Timer30=0,Role = {},BlueFlag = {},RedFlag={},BuffTrigger = {}}      --��¼�˿�ʼʱ�����ʧ�˶��ٸ�30���ʱ��
		Jidou.RoomInfo[InctancID].RedFlag = {State="home",Role=0,Time=0,FlagID=0,BianShen={1123501,1123501,1123601,1123601,1123401,1123401,1123801,1123801}}  --����������״̬��home��outside��������Ϊoutsideʱ������ڵ��ϣ���RoleΪ0�����������˭���ϡ��������ڵ���ʱ��Time��ÿ30���1�������������2��û����ȥ�����Ļ��������Լ��ص����С�
		Jidou.RoomInfo[InctancID].BlueFlag = {State="home",Role=0,Time=0,FlagID=0,BianShen={1123701,1123701,1123101,1123101,1123201,1123201,1123301,1123301}} --Bianshen��¼�������п��õı���BUFF��������ѡ��������һ������ɾ����
		Jidou.RoomInfo[InctancID].BuffTrigger = {}
		Jidou.RoomInfo[InctancID].BuffTrigger[380] ={CreID=0,CreTypeID=0,x=460,y=5573,z=107}--��¼ÿ���������ϵĹ����ID��TYPEID�����������ж�������������Ƿ���BUFF���Գ�
		Jidou.RoomInfo[InctancID].BuffTrigger[383] ={CreID=0,CreTypeID=0,x=79,y=5590,z=447}
		Jidou.RoomInfo[InctancID].BuffTrigger[388] ={CreID=0,CreTypeID=0,x=282,y=7302,z=284}
		Jidou.RoomInfo[InctancID].BuffTrigger[390] ={CreID=0,CreTypeID=0,x=195,y=5248,z=177}
		Jidou.RoomInfo[InctancID].BuffTrigger[396] ={CreID=0,CreTypeID=0,x=136,y=4448,z=102}
		for i=1,4 do
			Jidou.RoomInfo[InctancID].Role[tbl[i]] = {Name = tostring(role.GetRoleName(tbl[i])),Side = "Blue",Time = 1200,GetFlag = 0,MissFlag = 0, Kill = 0, Die = 0,Level = 0,Leave = 0} --level��¼�˳Ե��˶���BUFF���Ե�10����һ��.Leave��¼������Ƿ����뿪���ø���
		end
		for i=5,8 do
			Jidou.RoomInfo[InctancID].Role[tbl[i]] = {Name = tostring(role.GetRoleName(tbl[i])),Side = "Red",Time = 1200,GetFlag = 0,MissFlag = 0, Kill = 0, Die = 0,Level = 0,Leave = 0}
		end
	end
----------------------------------------------����������ˢ��-----------------------------------------------

    local function CreatNewMap(Index)

		map.CreateInstance(2108961076, Index)
		map.MapCreateCreature(2108961076,Index,4902135,1,1,1)

    end


------------------------------------------------���ͼ�״̬�ж�-----------------------------------------
    local function TransPlayerToMap(MapID, InstanceID, tbl)
		for k,v in pairs(tbl) do
			unit.CancelBuff(MapID, InstanceID,v, 1140901)        --ȡ��    ħ�񼤶��Ŷ���
			if role.IsRoleInStatus(v, 1024) then        --ȡ����̯
				role.CancelRoleStallState(MapID, InstanceID, v)
			end
			if unit.IsDead(MapID, InstanceID, v) then
				role.ForcedToRevive(MapID, InstanceID, v)
			end
			if k <= 4 then
				role.RoleGotoNewMap(MapID, InstanceID, v, 2108961076,81,112,426)--���͵��������
			else
				role.RoleGotoNewMap(MapID, InstanceID, v, 2108961076, 442,111,105)--���͵��췽���
			end
		end
	end
------------------------------------------------����Ϊ�Ի�---------------------------------------------
	if TalkIndex == -1 then
        if act.GetActIsStart(161) or act.GetActIsStart(162) then
		    local bool = 0
			for k,v in pairs(Jidou.Singlewaitinglist) do
				if v == RoleID then
					bool = 1
				end
			end
			for k,v in pairs(Jidou.Teamwaitinglist) do
				if v == RoleID then
					bool = 2
				end
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3701)		--��ѡ��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	3702)		--�����˱�����
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	3703)		--�����鱨����
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	12930)		    --��ȡ����
			if bool > 0 then
				msg.AddMsgEvent(MsgID, 21,	7)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	12931)			--��ȡ��������
			end
			msg.AddMsgEvent(MsgID, 21,	8)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12932)		--�û�п�ʼ�����ܱ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	12930)		    --��ȡ����
			msg.AddMsgEvent(MsgID, 21,	8)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif TalkIndex == 4 then
        if act.GetActIsStart(161) or act.GetActIsStart(162) then
			--���������ж�
			--�Ƿ��Ѿ�����
			local bool = true
			for k,v in pairs(Jidou.Singlewaitinglist) do
				if v == RoleID then
					bool = false
				end
			end
			for k,v in pairs(Jidou.Teamwaitinglist) do
				if v == RoleID then
					bool = false
				end
			end

			if not bool then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3704)		--�����ظ�����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif rolelevel<50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3705)		--50��������Ҳ��ɲμ�ħ�񼤶�
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif Jidou_Times>=5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3706)		--�����Ѳμӹ�5�μ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else

				--����Ҽ���ȴ��б�

				table.insert(Jidou.Singlewaitinglist,RoleID)
				unit.AddBuff(MapID, InstanceID, RoleID, 1140901, RoleID)        --Ϊ������һ��״̬��ħ�񼤶��Ŷ��С���״̬�л������������ߺ���ʧ

				if #Jidou.Singlewaitinglist == 8 then

					Jidou.RoomIndex = Jidou.RoomIndex + 1

					--��������
					CreatNewMap(Jidou.RoomIndex)

					--������Ӫ
					local tbl = {}
					for i = 1,4 do
						tbl[i] = Jidou.Singlewaitinglist[i*2-1]
					end
					for i = 5,8 do
						tbl[i] = Jidou.Singlewaitinglist[(i-4)*2]
					end

					--Ϊ������
					MSJD_JoinTeam(MapID, InstanceID, tbl)

					--�趨Ĭ��ֵ
					MSJD_VALUE(Jidou.RoomIndex,tbl)

					--Ϊÿ��û�й���Ĵ�����ˢһ������
                    for k,v in pairs(Jidou.RoomInfo[Jidou.RoomIndex].BuffTrigger) do
					    if v.CreID == 0 then
						    v.CreTypeID = math.random(4) + 4902118
							v.CreID = map.MapCreateColCreature(MapID, InstanceID, v.CreTypeID, v.x, v.y, v.z, 1)
						end
					end

					--������ҽ�ս��
					TransPlayerToMap(MapID, InstanceID, tbl)

					Jidou.Singlewaitinglist = {}
				else

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 3707)			--���ѳɹ�����ħ�񼤶�������������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

				end

			end

		end
    elseif TalkIndex == 5 then
        if act.GetActIsStart(161) or act.GetActIsStart(162) then
			--���������ж�
			--�Ƿ��Ѿ�����

			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			if TeamID == 4294967295 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	12934)		--������Ӳ��ܱ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local LeaderID = team.GetTeamLeaderID(TeamID)
				if LeaderID ~= RoleID then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	12935)		--ֻ�жӳ����ܱ���
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	22)			--��������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local Role = {}
					Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
					local num = 0
					local bool = true
					local map_bool = true
					local level = true
					local times = true
					for i=1, 6 do
					    if Role[i] and Role[i] ~= 4294967295 then

							for k,v in pairs(Jidou.Singlewaitinglist) do
								if v == Role[i] then
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 20,	12936)		--�������Ѿ����˱���
									msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
									msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
									msg.AddMsgEvent(MsgID, 1,	22)			--��������
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
									bool = false
								end
							end
							for k,v in pairs(Jidou.Teamwaitinglist) do
								if v == Role[i] then
									local MsgID = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID, 20,	12936)		--�������Ѿ����˱���
									msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
									msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
									msg.AddMsgEvent(MsgID, 1,	22)			--��������
									msg.DispatchRoleMsgEvent(RoleID, MsgID)
									bool = false
								end
							end
							local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
							if Target_MapID ~= 3017298383 then
							    map_bool = false
							end
							if role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])< 50 then
							    level = false
							end
							local Jidou_Times = role.GetRoleScriptData(Role[i], 1, RoleDataType["Jidou_Times"])
							local Jidou_Last = role.GetRoleScriptData(Role[i], 1, RoleDataType["Jidou_Last"])

							local curdate = tonumber(os.date("%j"))

							if curdate ~= Jidou_Last then
								Jidou_Last = curdate
								role.SetRoleScriptData(Role[i], 1, RoleDataType["Jidou_Last"], Jidou_Last)
								role.SetRoleScriptData(Role[i], 1, RoleDataType["Jidou_Times"], 0)
							end
							if Jidou_Times >= 5 then
							    times = false
							end

							num = num + 1
						end
					end
					if bool then
					    if num ~= 4 then
						    local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20,	12937)		--��ҵ�����������4��
							msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
							msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
							msg.AddMsgEvent(MsgID, 1,	22)			--��������
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						elseif not map_bool then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20,	12938)		--����������Ҷ��ڷ�����ܱ���
							msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
							msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
							msg.AddMsgEvent(MsgID, 1,	22)			--��������
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						elseif not level then
						    local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20,	12939)		--�ȼ����붼��50����
							msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
							msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
							msg.AddMsgEvent(MsgID, 1,	22)			--��������
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						elseif not times then
				            local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20,	12940)		--���˽����Ѿ�����5��
							msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
							msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
							msg.AddMsgEvent(MsgID, 1,	22)			--��������
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						else

							--����Ҽ���ȴ��б�
							local maxnum = 4
							for i = 1,6 do
							    if Role[i] and Role[i] ~= 4294967295 and maxnum > 0 then
								    maxnum = maxnum - 1
									table.insert(Jidou.Teamwaitinglist,Role[i])
									unit.AddBuff(MapID, InstanceID, Role[i], 1140901, Role[i])        --Ϊ������һ��״̬��ħ�񼤶��Ŷ��С���״̬�л������������ߺ���ʧ
								end
							end

							if #Jidou.Teamwaitinglist == 8 then

								Jidou.RoomIndex = Jidou.RoomIndex + 1

								--��������
								CreatNewMap(Jidou.RoomIndex)

								--������Ӫ
								local tbl = {}
								for i = 1,8 do
									tbl[i] = Jidou.Teamwaitinglist[i]
								end

								--Ϊ������
								MSJD_JoinTeam(MapID, InstanceID, tbl)

								--�趨Ĭ��ֵ
								MSJD_VALUE(Jidou.RoomIndex,tbl)

								--Ϊÿ��û�й���Ĵ�����ˢһ������
								for k,v in pairs(Jidou.RoomInfo[Jidou.RoomIndex].BuffTrigger) do
									if v.CreID == 0 then
										v.CreTypeID = math.random(4) + 4902118
										v.CreID = map.MapCreateColCreature(MapID, InstanceID, v.CreTypeID, v.x, v.y, v.z, 1)
									end
								end

								--������ҽ�ս��
								TransPlayerToMap(MapID, InstanceID, tbl)

								Jidou.Teamwaitinglist = {}
							else
								for i = 1,6 do
									if Role[i] and Role[i] ~= 4294967295 then
										local MsgID = msg.BeginMsgEvent()
										msg.AddMsgEvent(MsgID, 71, 3707)			--���ѳɹ�����ħ�񼤶�������������
										msg.DispatchRoleMsgEvent(RoleID, MsgID)
									end
								end
							end
						end
					end
				end
			end
		end
	elseif TalkIndex == 6 then
		--�����������9999���ĵ�������
        local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["MSJD_Award"])
		if k <= 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12941)		--��û�п�����ȡ�Ľ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12942)		--����XX�������죬����ձ�������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	k)
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12943)		--���Ѿ���ȡ��XX��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	k)
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    role.AddRoleItem(MapID, InstanceID, RoleID, 6010840, k, -1, 8, 420)    --ħ֮����δ��ID
			role.SetRoleScriptData(RoleID, 1, RoleDataType["MSJD_Award"],0)
		end
	elseif TalkIndex == 7 then
        local bool = 0
		local index = 0
		for k,v in pairs(Jidou.Singlewaitinglist) do
			if v == RoleID then
				bool = 1
				index = k
			end
		end
		for k,v in pairs(Jidou.Teamwaitinglist) do
			if v == RoleID then
				bool = 2
				index = k
			end
		end
		if bool == 1 then
		    unit.CancelBuff(MapID, InstanceID,RoleID, 1140901) --ȡ��ħ���Ŷ�״̬
		    table.remove(Jidou.Singlewaitinglist,index)
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 12909)			--�����뿪��ħ�񶷼������Ŷ�����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif bool == 2 then
		    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			if TeamID ~= 4294967295 then
			    local LeaderID = team.GetTeamLeaderID(TeamID)
				if LeaderID ~= RoleID then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	12945)		--ֻ�жӳ�����ȡ��
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
					msg.AddMsgEvent(MsgID, 1,	22)			--��������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					if index < 5 then
						for i = 1,4 do
						    local i = 5-i          --Ϊ�˴Ӻ���ǰɾ�����
						    local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 71, 12909)			--�����뿪��ħ�񶷼������Ŷ�����
							msg.DispatchRoleMsgEvent(Jidou.Teamwaitinglist[i], MsgID)
							unit.CancelBuff(MapID, InstanceID,Jidou.Teamwaitinglist[i], 1140901)--ȡ��ħ���Ŷ�״̬
							table.remove(Jidou.Teamwaitinglist,i)
						end
					elseif index < 9 then
			            for i = 5,8 do
                            local i = 13-i			 --Ϊ�˴Ӻ���ǰɾ�����
						    local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 71, 12909)			--�����뿪��ħ�񶷼������Ŷ�����
							msg.DispatchRoleMsgEvent(Jidou.Teamwaitinglist[i], MsgID)
							unit.CancelBuff(MapID, InstanceID,Jidou.Teamwaitinglist[i],1140901 )--ȡ��ħ���Ŷ�״̬
							table.remove(Jidou.Teamwaitinglist,i)
						end
					end
				end
			end
		end

	end
end




function moshenchuanchu(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local ChuanchuPos = {}
	ChuanchuPos[1] = {x=866,y=10000,z=1994}
	ChuanchuPos[2] = {x=719,y=10000,z=2098}
	ChuanchuPos[3] = {x=1080,y=10000,z=2130}
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	12947)		--NPC������Ի�����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)
		msg.AddMsgEvent(MsgID, 1,	12948)		--�˳�ս��
		msg.AddMsgEvent(MsgID, 21,	5)
		msg.AddMsgEvent(MsgID, 1,	22)			--ȡ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local i = math.random(3)
		role.RoleGotoNewMap(MapID, InstanceID,RoleID,3017298383,ChuanchuPos[i].x ,ChuanchuPos[i].y ,ChuanchuPos[i].z)
	end
end
function CreatNPC_MSJD(MapID,InstanceID,CreTypeID,CreAi)

		local bianshen = {}
		bianshen[4902132] = {BuffID=1123701,x=62,y=5588,z=402}--����3������NPC
		bianshen[4902109] = {BuffID=1123101,x=62,y=5582,z=407}--����3������NPC
		bianshen[4902110] = {BuffID=1123201,x=58,y=5574,z=419}--����3������NPC
		bianshen[4902111] = {BuffID=1123301,x=58,y=5566,z=425}--����3������NPC
		bianshen[4902106] =	{BuffID=1123501,x=460,y=5550,z=129}--�췽3������NPC
		bianshen[4902107] =	{BuffID=1123601,x=462,y=5537,z=120}--�췽3������NPC
		bianshen[4902108] =	{BuffID=1123401,x=452,y=5376,z=80}--�췽3������NPC
		bianshen[4902131] =	{BuffID=1123801,x=450,y=5348,z=75}--�췽3������NPC

		for k,v in pairs(bianshen) do
			local creid = map.MapCreateCreature(MapID, InstanceID, k, v.x, v.y, v.z)--����3������NPC
		    cre.SetCreatureScriptData(MapID, InstanceID, creid, 1, 0, v.BuffID)      --����NPC�ĵ�0��λ�ü�¼����Ӧ��ʲôBUFF
		end

		map.MapCreateCreature(MapID, InstanceID, 4902112,97,5556,406)--��������
		map.MapCreateCreature(MapID, InstanceID, 4902114,64,5555,440)--��������
		map.MapCreateCreature(MapID, InstanceID, 4902113,425,5483,109)--�췽����
		map.MapCreateCreature(MapID, InstanceID, 4902133,464,5517,96)--�췽���죬���ߵĿ��첻��ͬ���Ŀ���ˣ�����һ�������ID

		map.MapCreateCreature(MapID, InstanceID, 4902134, 1, 1, 1)--ˢ��һ��30���͸����ʱ�����ù���δ������
end

aux.RegisterCreatureEvent(4902135, 13, "CreatNPC_MSJD")--��ʱ����ʧ�¼�ע��
aux.RegisterCreatureEvent(4902118, 7, "moshenchuanchu")
aux.RegisterCreatureEvent(4902101, 7, "moshenchuansong")
aux.RegisterCreatureEvent(4902102, 7, "moshenchuansong")
aux.RegisterCreatureEvent(4902103, 7, "moshenchuansong")



