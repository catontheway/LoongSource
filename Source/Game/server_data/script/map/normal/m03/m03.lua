
m03_Trigger = {} -- ��¼trigger����Ƶ���
m03_Trigger[1] = {ID = 6631, Time = 0}
m03_Trigger[2] = {ID = 6632, Time = 0}
m03_Trigger[3] = {ID = 6634, Time = 0}
m03_Trigger[4] = {ID = 6635, Time = 0}
m03_Trigger[5] = {ID = 6636, Time = 0}
m03_Trigger[6] = {ID = 6637, Time = 0}

--��¼m03��ͼScriptData����ͬʱ���ڵ�����
--m03��ͼ����1��11279 ����ͯ��[1004607]��¼����
--m03��ͼ����2��11905 ��Ŀ��Գ[1534134]��¼����

--������ʧ������������ScriptData����1 ��ֵ��1
function m03_OnDisappear(MapID,InstanceID,CreatureID,CreatureTypeID)
	if CreatureTypeID == 1004607 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,1)
		map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
	end
	if CreatureTypeID == 1534134 then  --11905
		local a = map.GetMapScriptData(MapID,InstanceID,1,2)
		map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
	end
end
aux.RegisterMapEvent("m03", 18, "m03_OnDisappear")


--���������11305�����봥����id��6631 (m03,543,13468,902),ˢ�¹��� ð�Ʒ˾�[1004249]X5,ð�Ʒ���[1004250]X1
function m03_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	local bHaveQuest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 11305)
	if bHaveQuest then
		if ObjID == 6631 then
		local cTime = os.time() --��ǰʱ��
		local dTime = cTime - m03_Trigger[1].Time --�ϴδ���������ʱ��
			if dTime > 180 then --�жϴ�������Ƿ����3����
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+4, y+1, z-2)

			map.MapCreateCreature(MapID, InstanceID, 1004250, x, y+1, z+2)
			m03_Trigger[1].Time = os.time() --��¼�˴�����������Чʱ��
			end

		--���������11305�����봥����id��6632 (m03,621,11250,658),ˢ�¹��� ð�Ʒ˾�[1004249]X5,ð�Ʒ���[1004250]X1
		elseif ObjID == 6632 then
			local cTime = os.time() --��ǰʱ��
			local dTime = cTime - m03_Trigger[2].Time --�ϴδ���������ʱ��
			if dTime > 180 then --�жϴ�������Ƿ����3����
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+4, y+1, z-2)

			map.MapCreateCreature(MapID, InstanceID, 1004250, x, y+1, z+2)
			m03_Trigger[2].Time = os.time() --��¼�˴�����������Чʱ��
			end

		--���������11305�����봥����id��6634 (m03,863,9428,566),ˢ�¹��� ð�Ʒ˾�[1004249]X5,ð�Ʒ���[1004250]X1
		elseif ObjID == 6634 then
			local cTime = os.time() --��ǰʱ��
			local dTime = cTime - m03_Trigger[3].Time --�ϴδ���������ʱ��
			if dTime > 180 then --�жϴ�������Ƿ����3����
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004250, x, y+1, z+2)
			m03_Trigger[3].Time = os.time() --��¼�˴�����������Чʱ��
			end

		--���������11305�����봥����id��6635 (m03,900,7692,745),ˢ�¹��� ð�Ʒ˾�[1004249]X5,ð�Ʒ���[1004250]X1
		elseif ObjID == 6635 then
			local cTime = os.time() --��ǰʱ��
			local dTime = cTime - m03_Trigger[4].Time --�ϴδ���������ʱ��
			if dTime > 180 then --�жϴ�������Ƿ����3����
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+4, y+1, z-2)

			map.MapCreateCreature(MapID, InstanceID, 1004250, x, y+1, z+2)

			m03_Trigger[4].Time = os.time() --��¼�˴�����������Чʱ��
			end

		--���������11305�����봥����id��6636 (m03,422,13918,994),ˢ�¹��� ð�Ʒ˾�[1004249]X5,ð�Ʒ���[1004250]X1
		elseif ObjID == 6636 then
			local cTime = os.time() --��ǰʱ��
			local dTime = cTime - m03_Trigger[5].Time --�ϴδ���������ʱ��
			if dTime > 180 then --�жϴ�������Ƿ����3����
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004250, x, y+1, z+2)
			m03_Trigger[5].Time = os.time() --��¼�˴�����������Чʱ��
			end

		--���������11305�����봥����id��6637 (m03,490,14114,829),ˢ�¹��� ð�Ʒ˾�[1004249]X5,ð�Ʒ���[1004250]X1
		elseif ObjID == 6637 then
		local cTime = os.time() --��ǰʱ��
		local dTime = cTime - m03_Trigger[6].Time --�ϴδ���������ʱ��
			if dTime > 180 then --�жϴ�������Ƿ����3����
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x-2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+2, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004249, x+4, y+1, z-2)
			map.MapCreateCreature(MapID, InstanceID, 1004250, x, y+1, z+2)
			m03_Trigger[6].Time = os.time() --��¼�˴�����������Чʱ��
			end

		end
	end
end
aux.RegisterMapEvent("m03",6,"m03_OnEnterTrigger")
